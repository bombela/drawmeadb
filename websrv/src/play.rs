use rocket::fs::NamedFile;
use rocket::serde::{json::Json, Serialize};
use rocket::State;
use std::fs;
use std::path::Path;

mod solved_id;
use solved_id::SolvedID;

const VAR_DIR: &str = "/tmp/dmadb";

pub fn routes() -> Vec<rocket::Route> {
    routes![index, solve, solved_file]
}

#[get("/")]
fn index() -> &'static str {
    "Time to play"
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
        Err(e) => Json(SolveResponse::Error(e)),
    }
}

async fn _solve(assignment: String, chatgpt_api_key: &str) -> Result<SolvedID, String> {
    let mut id = SolvedID::random();
    let mut attempt = 0;
    for _ in 1..=5 {
        attempt += 1;
        let dir = Path::new(VAR_DIR).join(id.as_str());
        let answer = if chatgpt_api_key.is_empty() {
            FAKE_CHATGPT_ANSWER.into()
        } else {
            ask_chatgpt(&assignment).await?
        };
        match fs::create_dir_all(&dir) {
            Ok(()) => (),
            Err(e) if e.kind() == std::io::ErrorKind::AlreadyExists => (),
            Err(e) => panic!("Cannot create '{dir:?}': {e}"),
        }
        fs::write(dir.join("assignment.txt"), &assignment).map_err(|e| e.to_string())?;
        fs::write(dir.join("answer.txt"), &answer).map_err(|e| e.to_string())?;
        match run_merise_acide(&dir).await {
            Ok(()) => return Ok(id),
            Err(e) => {
                fs::write(dir.join("error.txt"), &e).map_err(|e| e.to_string())?;
            }
        }
        id.inc();
    }
    Err(format!(
        "ChatGPT couldn't return an usable solution after {} attempt(s)",
        attempt
    ))
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
    cmd.args(&["-f", "png", "-t", "SQLite"]);
    cmd.args(&["-c", "-E", "#FF0000", "-A", "#00FF00", "-B", "#0000FF"]);
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

async fn ask_chatgpt(question: &str) -> Result<String, String> {
    Ok("#test".into())
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
