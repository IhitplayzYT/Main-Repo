use tokio::process::Command;

pub async fn exec_bin(args: Vec<String>) -> Result<String,String> {
let mut exe = Command::new("../../Goscrapper");
exe.args(args);
let ret = exe.output().await.unwrap();


Ok("Success".to_string())
}
    

