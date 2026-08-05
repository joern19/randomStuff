use std::{error::Error, fmt::Display};

#[derive(Debug, Clone)]
pub struct CError {
  pub msg: String,
}

impl Error for CError {}

impl Display for CError {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{}", self.msg)
  }
}
