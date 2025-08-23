from pathlib import Path

class S3Client:
    def __init__(self, bucket: str):
        self.bucket = bucket

    def upload(self, local_path: str, key: str) -> bool:
        path = Path(local_path)
        if not path.exists():
            return False
        print(f"[s3] upload {local_path} -> s3://{self.bucket}/{key}")
        return True

    def download(self, key: str, local_path: str) -> bool:
        Path(local_path).write_text("stub")
        print(f"[s3] download s3://{self.bucket}/{key} -> {local_path}")
        return True
