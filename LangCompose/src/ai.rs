pub mod LLM {
    use crate::helper::Helper::MODEL;
    use reqwest::blocking::Client;
    use serde::{Deserialize, Serialize};
    const source_lang: &str = "english";

    #[derive(Serialize)]
    struct OllamaRequest<'a> {
        model: &'a str,
        prompt: String,
        stream: bool,
    }

    #[derive(Deserialize)]
    struct OllamaResponse {
        response: String,
    }

    pub fn translate_all(
        texts: &[String],
        target_lang: &str,
    ) -> Result<Vec<String>, Box<dyn std::error::Error>> {
        let json_input = serde_json::to_string_pretty(texts)?;

        let prompt = format!(
            r#"You are a professional translator.You can be provided with plain text or format string text

Translate each string from {} to {}.

Rules:
- Preserve array order.
- Return ONLY valid JSON.
- Return a JSON array of strings.
- Do not add explanations.
- Do not add markdown.
- Remove enclosing ``,"",''
- Do not translate numbers.
- Do not modify special characters.
- Do not modify format specifiers.
- Do not modify placeholder variables.
- Keep escaped characters intact.
- I dont need code i want direct translation done by you
- Translate only human-readable text.
- Donot translate contents in ${{}}
- If a word looks like a name or not in the vocabulary assume it as special and translate by pronounciation/dialect
Input:
{}

Output format:
[
  "...",
  "..."
]
"#,
            source_lang, target_lang, json_input
        );

        let req = OllamaRequest {
            model: &MODEL.try_read().unwrap().to_string()[..],
            prompt,
            stream: false,
        };

        let client = Client::new();
        println!("Sending {source_lang} {target_lang} {json_input}");
        let resp: OllamaResponse = client
            .post("http://localhost:11434/api/generate")
            .json(&req)
            .send()?
            .json()?;
        println!("Done");
        println!("{}", resp.response);
        let translated: Vec<String> = serde_json::from_str(resp.response.trim())?;

        Ok(translated)
    }

    pub fn translate(text: &str, target_lang: &str) -> Result<String, Box<dyn std::error::Error>> {
        let prompt = format!(
            r#"You are a professional translator.You are provided with plain text or a javascript style format string

Translate the following text from {} to {}.

Return ONLY the translated text.
If it is a format string donot translate format specifiers or placeholder variable names
Do not translate numbers or special chaecters only alphabets
Do not explain anything.
Do not add quotes.
Do not add markdown.
Make sure the translation is accurate and slang free
TEXT:
{}"#,
            source_lang, target_lang, text
        );

        let req = OllamaRequest {
            model: &MODEL.try_read().unwrap().to_string()[..],
            prompt,
            stream: false,
        };

        let client = Client::new();

        let resp: OllamaResponse = client
            .post("http://localhost:11434/api/generate")
            .json(&req)
            .send()?
            .json()?;

        Ok(resp.response.trim().to_string())
    }
}

