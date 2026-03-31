import os
import requests
import subprocess
import re

def downloadMaps(base_url, output_dir, file_num):
    print("Downloading maps...\n")
    for i in range(1, file_num + 1):
        map_url = base_url + "map" + str(i) + ".tmx"
        try:
            subprocess.run(["wget", "-nc", "-q", "-P", output_dir, map_url])
        except:
            pass
    print("Done!\n")

def checkFilesCount(base_url):
    response = requests.get(base_url)
    text = response.text

    matches = re.findall(r"map(\d+)\.tmx", text)
    numbers = [int(x) for x in matches]

    return max(numbers) if numbers else 0

if __name__ == "__main__":

    base_url = "https://alas.matf.bg.ac.rs/~mr22033/levels/uploads/"
    output_dir = "maps"

    file_num = checkFilesCount(base_url)

    os.makedirs(output_dir, exist_ok=True)
    downloadMaps(base_url, output_dir, file_num)

    try:
        with open("src/levelCount.txt", "w") as file:
            file.write(str(file_num))
    except:
        print("Can not get the number of levels")
    finally:
        file.close()

    subprocess.run(["mingw32-make", "all"]) # compile project LaserTankVSCode
    subprocess.run(["LaserTank.exe"])     # run program
