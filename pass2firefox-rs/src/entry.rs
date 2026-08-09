use serde::Serialize;

#[derive(Serialize)]
pub struct Entry {
  pub path: String,
  pub password: String,
  pub username: String,
  pub url: String,
}
