# prebuild.py
Import("env")
import subprocess

print("🔧 Generating certs...")
subprocess.run(["python", "generate_certs.py"], check=True)
