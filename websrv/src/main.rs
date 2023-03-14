use rocket::{
    fairing::{Fairing, Info, Kind},
    http::Header,
    Request, Response, serde::Deserialize,
};

#[macro_use]
extern crate rocket;

mod play;

#[get("/")]
fn index() -> &'static str {
    "Hello, world!"
}

#[launch]
fn rocket() -> _ {
    let mut rocket = rocket::build();

    #[derive(Deserialize)]
    #[serde(crate = "rocket::serde")]
    struct Config {
        cors_all: bool,
    }

    let config: Config = rocket.figment().extract().unwrap();
    if config.cors_all {
        rocket = rocket.attach(Cors);
    }
    rocket
        .mount("/", routes![index])
        .mount("/play", play::routes())
}

pub struct Cors;

#[rocket::async_trait]
impl Fairing for Cors {
    fn info(&self) -> Info {
        Info {
            name: "Cross-Origin-Resource-Sharing Fairing",
            kind: Kind::Response,
        }
    }

    async fn on_response<'r>(&self, _request: &'r Request<'_>, response: &mut Response<'r>) {
        response.set_header(Header::new("Access-Control-Allow-Origin", "*"));
    }
}
