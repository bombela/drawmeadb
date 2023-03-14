use rand::{thread_rng, Rng};
use rocket::{request::FromParam, serde::{Serialize, self}};

/// The (unique enough) ID of a solved assignment.
/// New IDs are random. They are ASCII Alphanumeric.
/// The last character is always a digit, starting at '0'.
/// This way, we can increment the ID to represent a new attempt.
pub struct SolvedID(Box<[u8]>);

impl FromParam<'_> for SolvedID {
    type Error = ();

    // Invariant: SolvedID is always valid alphanumeric ASCII ending with a digit.
    fn from_param(param: &'_ str) -> Result<Self, Self::Error> {
        if param.len() == Self::LEN
            && param.is_ascii()
            && param.as_bytes().last().unwrap().is_ascii_digit()
        {
            Ok(SolvedID(param.as_bytes().to_vec().into_boxed_slice()))
        } else {
            Err(())
        }
    }
}

impl SolvedID {
    const LEN: usize = 15;

    pub fn random() -> Self {
        Self(
            thread_rng()
                .sample_iter(&rand::distributions::Alphanumeric)
                .take(Self::LEN - 1)
                .chain(std::iter::once(b'0'))
                .collect::<Vec<u8>>()
                .into_boxed_slice(),
        )
    }

    // Increment the last digit of the ID to a maximum of 9.
    pub fn inc(&mut self) {
        if let Some(c) = self.0.last_mut() {
            if *c < b'9' {
                *c += 1;
            }
        }
    }

    pub fn as_str(&self) -> &str {
        // SAFETY: SolvedID is always valid alphanumeric ASCII ending with a digit.
        unsafe { std::str::from_utf8_unchecked(&self.0) }
    }
}

impl ToString for SolvedID {
    fn to_string(&self) -> String {
        self.as_str().to_string()
    }
}

impl Serialize for SolvedID {
    fn serialize<S: serde::Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        serializer.serialize_str(self.as_str())
    }
}
