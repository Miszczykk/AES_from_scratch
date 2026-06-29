import subprocess
import os
import time
import base64
import random
import csv
from Crypto.Cipher import AES

project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
exe_name = 'AES_from_scratch.exe' if os.name == 'nt' else 'AES_from_scratch'
exe_path = os.path.join(project_root, 'cmake-build-debug', exe_name)

if not os.path.exists(exe_path):
    print(f"Error: don't find file {exe_path}.")
    exit(1)

def test_aes(public_text: str, secret_key: str):
    start_time = time.time()

    result = subprocess.run(
        [exe_path, public_text, secret_key],
        capture_output=True,
        text=True,
        errors='replace'
    )

    end_time = time.time()

    output = result.stdout.strip()
    output = output.replace("\x1b[1t\x1b[c\x1b[?1004h\x1b[?9001h", "")
    lines = output.split('\n')
    if len(lines) >= 2:
        encryption = lines[0].strip()
        decryption = lines[1].strip()
    else:
        encryption = output
        decryption = ""

    execution_time = end_time - start_time

    return encryption, decryption, execution_time

def get_python_aes_encryption(plain_text: str, secret_key: str) -> str:
    pt_bytes = plain_text.encode('utf-8')
    key_bytes = secret_key.encode('utf-8')

    key_bytes = key_bytes.ljust(16, b'\x00')[:16]

    padding_length = 16 - (len(pt_bytes) % 16)
    if padding_length != 16:
        pt_bytes += b'\x00' * padding_length
    elif len(pt_bytes) == 0:
        pt_bytes = b'\x00' * 16

    cipher = AES.new(key_bytes, AES.MODE_ECB)
    encrypted_bytes = cipher.encrypt(pt_bytes)

    return encrypted_bytes.hex()

if __name__ == "__main__":
    data = [
        ['RESULT' ,'TEXT_LENGTH', 'TIME']
    ]

    positive = 0

    for x in range(1_000):
        length_text = random.randrange(1, 9_999)
        length_key = random.randrange(1, 20)
        text_to_check = base64.b64encode(os.urandom(length_text)).decode('utf-8')[:length_text]
        key = base64.b64encode(os.urandom(16)).decode('utf-8')[:length_key]
        key = key.ljust(16, '0')

        console_result_encryption, console_result_decryption, time_result = test_aes(text_to_check, key)
        expected_encryption = get_python_aes_encryption(text_to_check, key)

        is_decryption_correct = (text_to_check == console_result_decryption.strip())
        is_encryption_correct = (expected_encryption == console_result_encryption.strip())

        result = is_decryption_correct and is_encryption_correct

        if result:
            positive+=1
        else:
            if not is_encryption_correct:
                print(f"[{x}] ERROR, length text: {length_text}, length key: {length_key}")
                print(f"Expected: {expected_encryption}")
                print(f"Get: {console_result_encryption}")
                print('-' * 50)
                time.sleep(2)


        data.append([result, length_text, time_result])

    print(f"\nPositive: {positive}")
    print(f"Negative: {1_000-positive}")
    with open('data.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter='|')
        writer.writerows(data)