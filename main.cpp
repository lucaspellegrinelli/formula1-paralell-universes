#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

#define standings_table std::unordered_map<std::string, std::unordered_map<std::string, std::pair<int, bool>>>

class DriverResults {
public:
  int winning_universes;
  std::vector<std::string> longest_winning_season;

  DriverResults() {
    this->winning_universes = 0;
  }
};

std::vector<std::string> strsplit(std::string s, std::string delim) {
  std::vector<std::string> out;
  size_t start;
  size_t end = 0;
  while ((start = s.find_first_not_of(delim, end)) != std::string::npos) {
    end = s.find(delim, start);
    out.push_back(s.substr(start, end - start));
  }

  return out;
}

int standing_points(int position, bool fastest_lap) {
  int total_points = fastest_lap;
  if (position == 1) total_points += 25;
  else if (position == 2) total_points += 18;
  else if (position == 3) total_points += 15;
  else if (position == 4) total_points += 12;
  else if (position == 5) total_points += 10;
  else if (position == 6) total_points += 8;
  else if (position == 7) total_points += 6;
  else if (position == 8) total_points += 4;
  else if (position == 9) total_points += 2;
  else if (position == 10) total_points += 1;
  return total_points;
}

std::vector<std::pair<std::string, int>> calculate_standings(std::vector<std::string> tracks, standings_table drivers) {
  std::unordered_map<std::string, int> total_points;
  std::unordered_map<std::string, std::vector<int>> classification_count;

  for (std::pair<std::string, std::unordered_map<std::string, std::pair<int, bool>>> driver : drivers) {
    std::vector<int> classif(drivers.size(), 0);
    classification_count[driver.first] = classif;
    total_points[driver.first] = 0;

    for (std::string track : tracks) {
      if (driver.second.find(track) != driver.second.end()) {
        std::pair<int, bool> standing = driver.second[track];
        classification_count[driver.first][standing.first - 1]++;
        total_points[driver.first] += standing_points(standing.first, standing.second);
      }
    }
  }

  std::vector<std::pair<std::string, int>> total_points_items;
  for (std::pair<std::string, int> driver_points : total_points) {
    total_points_items.push_back(driver_points);
  }

  std::sort(total_points_items.begin(), total_points_items.end(), [&](const auto &a, const auto &b) {
    if (a.first == "" || b.first == "") return false;
    if (a.second < b.second) {
      return false;
    } else if (a.second > b.second) {
      return true;
    } else {
      for (int i = 0; i < drivers.size(); i++) {
        if (classification_count[a.first][i] < classification_count[b.first][i])
          return false;
        else if (classification_count[a.first][i] > classification_count[b.first][i])
          return true;
      }

      return false;
    }
  });

  return total_points_items;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Erro. Número de parâmetros incorreto." << std::endl;
    exit(1);
  }

  std::ifstream table_file;
  table_file.open(argv[1]);

  if (!table_file) {
    std::cout << "Erro. Arquivo não pode ser aberto." << std::endl;
    exit(1);
  }

  std::string header;
  std::getline(table_file, header);
  std::vector<std::string> tracks = strsplit(header, " ");

  standings_table drivers;
  std::string line;
  while (std::getline(table_file, line)) {
    std::unordered_map<std::string, std::pair<int, bool>> standings_map;
    std::vector<std::string> line_parts = strsplit(line, "\" ");
    std::vector<std::string> standings = strsplit(line_parts[1], " ");
    for (int i = 0; i < standings.size(); i++) {
      if (standings[i][0] != '-') {
        bool isFastestLap = standings[i][standings[i].size() - 1] == 'F';
        int position = std::stoi(standings[i].substr(0, standings[i].size() - isFastestLap));
        standings_map[tracks[i]] = { position, isFastestLap };
      }
    }

    drivers[line_parts[0]] = standings_map;
  }

  std::unordered_map<std::string, DriverResults> driver_data;
  for (int r = 1; r <= tracks.size(); r++) {
    std::cout << "Calculating " << r << std::endl;
    std::vector<bool> v(tracks.size());
    std::fill(v.end() - r, v.end(), true);
    do {
      std::vector<std::string> chosen_tracks;
      for (int i = 0; i < tracks.size(); ++i) {
        if (v[i]) chosen_tracks.push_back(tracks[i]);
      }

      std::vector<std::pair<std::string, int>> result = calculate_standings(chosen_tracks, drivers);
      std::string winner = result[0].first;

      if (driver_data.find(winner) == driver_data.end()) {
        driver_data[winner] = DriverResults();
      }

      driver_data[winner].winning_universes++;
      if (r > driver_data[winner].longest_winning_season.size())
        driver_data[winner].longest_winning_season = chosen_tracks;

    } while (std::next_permutation(v.begin(), v.end())); 
  }

  std::vector<std::pair<std::string, DriverResults>> driver_data_items;
  for(std::pair<std::string, DriverResults> item : driver_data) {
    driver_data_items.push_back(item);
  }

  std::sort(driver_data_items.begin(), driver_data_items.end(), [](const auto &a, const auto &b) {
    return a.second.winning_universes > b.second.winning_universes;
  });

  for (std::pair<std::string, DriverResults> result : driver_data_items) {
    std::cout << result.first << std::endl;
    std::cout << " > " << result.second.winning_universes << " winning universes" << std::endl;
    std::cout << " > Longest winning season: ";
    for (std::string track : result.second.longest_winning_season) {
      std::cout << track << " ";
    }
    std::cout << std::endl;

    std::vector<std::pair<std::string, int>> standings = calculate_standings(result.second.longest_winning_season, drivers);
    for (int i = 0; i < 5; i++) {
      std::cout << "   - " << standings[i].first << ": " << standings[i].second << std::endl;
    }

    std::cout << std::endl;
  }
}