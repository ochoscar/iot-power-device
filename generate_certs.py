from pathlib import Path
import re

constants_path = Path("include/DeviceConstants.h")

def extract_device_id(header_path):
    pattern = re.compile(r'#define\s+DEVICE_ID\s+"([^"]+)"')
    with open(header_path, "r") as file:
        for line in file:
            match = pattern.search(line)
            if match:
                return match.group(1)
    raise ValueError("DEVICE_ID not found in DeviceConstants.h")
device_id = extract_device_id(constants_path)
print(f"📦 DEVICE_ID found: {device_id}")

cert_folder = Path("certs") / device_id

matching_files_aws = list(cert_folder.glob("AmazonRootCA1.pem"))
if len(matching_files_aws) != 1:
    print(f"❌ Error: only one aws cert expected but got {len(matching_files_aws)} in {cert_folder}")
    exit(1)
cert_path_aws = matching_files_aws[0]
aws_cert_ca = cert_path_aws.read_text().strip()

matching_files_crt = list(cert_folder.glob("*certificate.pem.crt"))
if len(matching_files_crt) != 1:
    print(f"❌ Error: only one crt cert expected but got {len(matching_files_crt)} in {cert_folder}")
    exit(1)
cert_path_crt = matching_files_crt[0]
cert_text_crt = cert_path_crt.read_text().strip()

matching_files_private = list(cert_folder.glob("*private.pem.key"))
if len(matching_files_private) != 1:
    print(f"❌ Error: only one private cert expected but got {len(matching_files_private)} in {cert_folder}")
    exit(1)
cert_path_private = matching_files_private[0]
cert_text_private = cert_path_private.read_text().strip()


header_content = f'''\
#ifndef CERTS_H
#define CERTS_H

#include <pgmspace.h>

static const char AWS_CERT_CA[] PROGMEM = R"EOF(
{aws_cert_ca}
)EOF";

static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
{cert_text_crt}
)KEY";

static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
{cert_text_private}
)KEY";

#endif
'''

output_file = Path("include/certs.h")
output_file.write_text(header_content)

print(f"✅ Certs generated and saved in {output_file}")