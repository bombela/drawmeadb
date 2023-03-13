#[get("/")]
fn index() -> &'static str {
    "Time to play"
}

pub fn routes() -> Vec<rocket::Route> {
    routes![index]
}
