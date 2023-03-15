use reqwest::header;
use rocket::fs::NamedFile;
use rocket::serde::Deserialize;
use rocket::serde::{json::Json, Serialize};
use rocket::State;
use std::fs;
use std::path::Path;

mod solved_id;
use solved_id::SolvedID;

const VAR_DIR: &str = "/tmp/dmadb";

pub fn routes() -> Vec<rocket::Route> {
    routes![index, reload, solve, solved_file]
}

#[get("/")]
async fn index() -> Option<NamedFile> {
    NamedFile::open("public/index.html").await.ok()
}

#[get("/<id>")]
async fn reload(id: SolvedID) -> Option<NamedFile> {
    let _unused = id;
    NamedFile::open("public/index.html").await.ok()
}

#[derive(Serialize)]
#[serde(crate = "rocket::serde")]
enum SolveResponse {
    Solved(SolvedID),
    Error(String),
}

#[post("/solve", data = "<assignment>")]
async fn solve(assignment: String, config: &State<crate::Config>) -> Json<SolveResponse> {
    match _solve(assignment, &config.chatgpt_api_key).await {
        Ok(id) => Json(SolveResponse::Solved(id)),
        Err(e) => Json(SolveResponse::Error(e.to_string())),
    }
}

async fn _solve(
    assignment: String,
    chatgpt_api_key: &str,
) -> Result<SolvedID, Box<dyn std::error::Error>> {
    use std::io::Write;

    let instructions = fs::read_to_string("prompt.txt")?;

    let mut id = SolvedID::random();
    let mut attempt = 0;
    for _ in 1..=5 {
        attempt += 1;
        let dir = Path::new(VAR_DIR).join(id.as_str());
        println!("Solving for {}", dir.display());
        let answer = if chatgpt_api_key.is_empty() {
            FAKE_CHATGPT_ANSWER.into()
        } else {
            ask_chatgpt(instructions.clone(), assignment.clone(), chatgpt_api_key).await?
        };
        match fs::create_dir_all(&dir) {
            Ok(()) => (),
            Err(e) if e.kind() == std::io::ErrorKind::AlreadyExists => (),
            Err(e) => panic!("Cannot create '{dir:?}': {e}"),
        }
        fs::write(dir.join("assignment.txt"), &assignment)?;
        let mut answer_file = fs::File::create(dir.join("answer.txt"))?;
        for line in answer.lines() {
            // Massage the simplified answer from ChatGTP.
            if line.contains("@MANY") || line.contains("@ONE") {
                write!(
                    answer_file,
                    "\n{}\n", // Add an empty line before to please MeriseAcide.
                    line.replace("@MANY", "0,n").replace("@ONE", "0,1")
                )?;
            } else {
                write!(answer_file, "{}\n", line)?;
            }
        }
        match run_merise_acide(&dir).await {
            Ok(()) => return Ok(id),
            Err(e) => {
                fs::write(dir.join("error.txt"), &e)?;
            }
        }
        id.inc();
    }
    Err(format!(
        "ChatGPT couldn't return a usable solution after {} attempt(s)",
        attempt
    )
    .into())
}

/// On success, dir will be populated with:
/// - conceptual.png
/// - physical.png
/// - schema.txt
async fn run_merise_acide(dir: &Path) -> Result<(), String> {
    use rocket::tokio::task::spawn_blocking;
    use std::process::Command;

    let mut cmd = Command::new("./MeriseAcide");
    cmd.args(&["-m".into(), dir.join("conceptual.png")]);
    cmd.args(&["-p".into(), dir.join("physical.png")]);
    cmd.args(&["-s".into(), dir.join("schema.txt")]);
    cmd.args(&["-f", "png", "-t", "MySQL"]);
    cmd.args(&["-c", "-E", "#edf7fc", "-A", "#E2E2E2", "-B", "#FFFFFF"]);
    cmd.arg(dir.join("answer.txt"));

    let output = spawn_blocking(move || cmd.output()).await.unwrap();
    let output = match output {
        Ok(o) => o,
        Err(e) => panic!("Failed to run MeriseAcide: {e}"),
    };
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "MeriseAcide failed with status {:?})",
            String::from_utf8_lossy(&output.stderr)
        ))
    }
}

#[get("/<id>/<file>")]
async fn solved_file(id: SolvedID, file: &str) -> Option<NamedFile> {
    NamedFile::open(Path::new(VAR_DIR).join(id.as_str()).join(&file))
        .await
        .ok()
}

#[derive(Deserialize)]
#[serde(crate = "rocket::serde")]
struct ChatGPTResponse {
    choices: Vec<ChatGPTChoice>,
}

#[derive(Deserialize)]
#[serde(crate = "rocket::serde")]
struct ChatGPTChoice {
    message: ChatGPTMessage,
}

#[derive(Serialize, Deserialize)]
#[serde(crate = "rocket::serde")]
struct ChatGPTMessage {
    role: ChatGTPMessageRole,
    content: String,
}

#[derive(Serialize, Deserialize)]
#[serde(crate = "rocket::serde")]
#[allow(unused)]
enum ChatGTPMessageRole {
    #[serde(rename = "system")]
    System,
    #[serde(rename = "user")]
    User,
    #[serde(rename = "assistant")]
    Assistant,
}

#[derive(Serialize)]
#[serde(crate = "rocket::serde")]
struct ChatGPTCompletionRequest<'a> {
    model: &'a str,
    temperature: f32,
    max_tokens: u32,
    messages: Vec<ChatGPTMessage>,
}

#[derive(Deserialize)]
#[serde(crate = "rocket::serde")]
struct ChatGPTError {
    error: ChatGPTErrorMessage,
}

#[derive(Deserialize)]
#[serde(crate = "rocket::serde")]
struct ChatGPTErrorMessage {
    message: String,
}

async fn ask_chatgpt(
    system: String,
    user: String,
    chatgpt_api_key: &str,
) -> Result<String, Box<dyn std::error::Error>> {
    // TODO cache client.
    let client = reqwest::Client::new();

    let request_body = ChatGPTCompletionRequest {
        model: "gpt-3.5-turbo",
        temperature: 0.9,
        max_tokens: 512,
        messages: vec![
            ChatGPTMessage {
                role: ChatGTPMessageRole::System,
                content: system,
            },
            ChatGPTMessage {
                role: ChatGTPMessageRole::User,
                content: user,
            },
        ],
    };

    const CHATGPT_API_URL: &str = "https://api.openai.com/v1/chat/completions";
    let request = client
        .post(CHATGPT_API_URL)
        .header(header::AUTHORIZATION, format!("Bearer {}", chatgpt_api_key))
        .header(header::CONTENT_TYPE, "application/json")
        .json(&request_body);
    let response = request.send().await?;
    if response.status() != 200 {
        let status = response.status();
        let error = response
            .json::<ChatGPTError>()
            .await
            .map(|e| e.error.message)
            .unwrap_or("N/A".into());
        return Err(format!("ChatGPT {status}: {error}").into());
    }
    let response: ChatGPTResponse = response.json().await?;
    let first_choice = response
        .choices
        .into_iter()
        .next()
        .ok_or("ChatGPT didn't return any answer")?;
    let answer = first_choice.message.content;
    Ok(answer)
}

const FAKE_CHATGPT_ANSWER: &str = r#"
# Conceptual data model for a movie rental system.

[Customer]
+CustomerID
Name
Address
PhoneNumber
EmailAddress

[Movie]
+MovieID
Title
ReleaseYear
Genre
Director
Length

[Rental]
+RentalID
RentalDate
DueDate
ReturnDate

[LateFee]
+LateFeeID
Amount
Date

# Association between Customer and Rental entities
# A customer can rent 0 or n movies, and a movie can be rented by 0 or 1 customers.
Customer    0,n    Rents       0,1    Movie

# Association between Rental and Customer entities
# A rental is associated with exactly one customer, and a customer can have 0 or n rentals.
Rental      1,1    AssociatedWith   0,n    Customer

# Association between Rental and Movie entities
# A rental is associated with exactly one movie, and a movie can have 0 or 1 rentals.
Rental      1,1    AssociatedWith2   0,1    Movie

# Association between Movie and LateFee entities
# A movie can have 0 or n late fees, and a late fee is associated with exactly one movie.
Movie       0,n    Has         1,1    LateFee
"#;
