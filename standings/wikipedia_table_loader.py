import requests
from bs4 import BeautifulSoup
import re

SEASON = 2017

def load_points_table(year):
  page = requests.get(f"https://en.wikipedia.org/wiki/{year}_Formula_One_World_Championship").text
  parsed_html = BeautifulSoup(page, features="lxml")
  table_title = parsed_html.find(id="World_Drivers'_Championship_standings")
  standings_table = table_title.findNext("table").findAll("tr")[0].findNext("table")
  st_rows = standings_table.findAll("tr")

  tracks = []
  for item in st_rows[0].findAll("th")[:-1]:
    if item.find("a"):
      tracks.append(item.find("a").text)

  drivers = {}
  for row in st_rows[1:]:
    try:
      items = row.findAll("td")
      driver = items[0].text.strip()
      drivers[driver] = {}
      for col, track in zip(items[1:], tracks):
        position = re.compile('\d+').findall(col.text)
        if len(position) > 0:
          drivers[driver][track] = { "position": int(position[0]), "fl": "F" in col.text }
    except:
      break

  return tracks, drivers

def create_table_file(season, tracks, drivers_points):
  with open(f"standings/points_table_{season}.txt", "w") as f:
    content = " ".join(tracks) + "\n"
    for driver, entries in drivers_points.items():
      content += f"\"{driver}\" "
      for track in tracks:
        if track in entries:
          content += str(entries[track]["position"])
          content += "F" if entries[track]["fl"] else ""
          content += " "
        else:
          content += "- "
      content += "\n"
    f.write(content)

def main():
  tracks, drivers_points = load_points_table(SEASON)
  create_table_file(SEASON, tracks, drivers_points)

if __name__ == "__main__":
  main()