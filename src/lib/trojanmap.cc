#include "trojanmap.h"

#include <limits.h>
#include <climits>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <locale>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <string>
#include <utility>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

//-----------------------------------------------------
// TODO (Students): You do not and should not change the following functions:
//-----------------------------------------------------

/**
 * PrintMenu: Create the menu
 * 
 */
void TrojanMap::PrintMenu() {

  std::string menu =
      "**************************************************************\n"
      "* Select the function you want to execute.                    \n"
      "* 1. Autocomplete                                             \n"
      "* 2. Find the position                                        \n"
      "* 3. CalculateShortestPath                                    \n"
      "* 4. Travelling salesman problem                              \n"
      "* 5. Exit                                                     \n"
      "**************************************************************\n";
  std::cout << menu << std::endl;
  std::string input;
  getline(std::cin, input);
  char number = input[0];
  switch (number)
  {
  case '1':
  {
    menu =
        "**************************************************************\n"
        "* 1. Autocomplete                                             \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "Please input a partial location:";
    std::cout << menu;
    getline(std::cin, input);
    auto results = Autocomplete(input);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    if (results.size() != 0) {
      for (auto x : results) std::cout << x << std::endl;
    } else {
      std::cout << "No matched locations." << std::endl;
    }
    menu = "**************************************************************\n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '2':
  {
    menu =
        "**************************************************************\n"
        "* 2. Find the position                                        \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "Please input a location:";
    std::cout << menu;
    getline(std::cin, input);
    auto results = GetPosition(input);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    if (results.first != -1) {
      std::cout << "Latitude: " << results.first
                << " Longitude: " << results.second << std::endl;
      PlotPoint(results.first, results.second);
    } else {
      std::cout << "No matched locations." << std::endl;
    }
    menu = "**************************************************************\n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '3':
  {
    menu =
        "**************************************************************\n"
        "* 3. CalculateShortestPath                                            "
        "      \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "Please input the start location:";
    std::cout << menu;
    std::string input1;
    getline(std::cin, input1);
    menu = "Please input the destination:";
    std::cout << menu;
    std::string input2;
    getline(std::cin, input2);
    auto results = CalculateShortestPath(input1, input2);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    if (results.size() != 0) {
      for (auto x : results) std::cout << x << std::endl;
      PlotPath(results);
    } else {
      std::cout << "No route from the start point to the destination."
                << std::endl;
    }
    menu = "**************************************************************\n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '4':
  {
    menu =
        "**************************************************************\n"
        "* 4. Travelling salesman problem                              \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "In this task, we will select N random points on the map and you need to find the path to travel these points and back to the start point.";
    std::cout << menu << std::endl << std::endl;
    menu = "Please input the number of the places:";
    std::cout << menu;
    getline(std::cin, input);
    int num = std::stoi(input);
    std::vector<std::string> keys;
    for (auto x : data) {
      keys.push_back(x.first);
    }
    std::vector<std::string> locations;
    srand(time(NULL));
    for (int i = 0; i < num; i++)
      locations.push_back(keys[rand() % keys.size()]);
    PlotPoints(locations);
    ///////////////////The begining of my modification/////////////////
    menu = 
        "**************************************************************\n"
        "* 1. Brute Force                                              \n"
        "* 2. 2-opt                                                    \n"
        "* 3. 3-opt                                                    \n"
        "**************************************************************\n";
    std::cout << menu << std::endl << std::endl;
    menu = "Please select your algorithm:";
    std::cout << menu;
    std::string inputm;
    getline(std::cin, inputm);
    int method = std::stoi(inputm);
    //std::cout << "your input:" << method << std::endl;
    std::pair<double, std::vector<std::vector<std::string>>> results;
    switch (method)
      {
        case 1:
          { 
            std::cout << "You choose Brute Force." << std::endl;
            std::cout << "Calculating ..." << std::endl;
            results = TravellingTrojan(locations);
            break;
          }
        case 2:
          {
            std::cout << "You choose 2-opt." << std::endl;
            std::cout << "Calculating ..." << std::endl;
            results = TravellingTrojan_2opt(locations);
            break;
          }
        case 3:
          {
            std::cout << "You choose 3-opt." << std::endl;
            std::cout << "Calculating ..." << std::endl;
            results = TravellingTrojan_3opt(locations);
            break;
          }
        default:
          {
            std::cout << "Invalid input, using default method: Brute Force" << std::endl;
            std::cout << "Calculating ..." << std::endl;
            results = TravellingTrojan(locations);
            break;
          }
      }
      //////////////The end of my modification////////////
    //std::cout << "Calculating ..." << std::endl;
    //auto results = TravellingTrojan_2opt(locations);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    CreateAnimation(results.second);
    if (results.second.size() != 0) {
      for (auto x : results.second[results.second.size()-1]) std::cout << x << std::endl;
      menu = "**************************************************************\n";
      std::cout << menu;
      std::cout << "The distance of the path is:" << results.first << std::endl;
      PlotPath(results.second[results.second.size()-1]);
    } else {
      std::cout << "The size of the path is 0" << std::endl;
    }
    menu = "**************************************************************\n"
           "You could find your animation at src/lib/output.avi.          \n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '5':
    break;
  default:
    std::cout << "Please select 1 - 5." << std::endl;
    PrintMenu();
    break;
  }
}


/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 * 
 */
void TrojanMap::CreateGraphFromCSVFile() {
  std::fstream fin;
  fin.open("src/lib/map.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '['), word.end());
      word.erase(std::remove(word.begin(), word.end(), ']'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}

/**
 * PlotPoint: Given a location id, plot the point on the map
 * 
 * @param  {std::string} id : location id
 */
void TrojanMap::PlotPoint(std::string id) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  auto result = GetPlotLocation(data[id].lat, data[id].lon);
  cv::circle(img, cv::Point(result.first, result.second), DOT_SIZE,
             cv::Scalar(0, 0, 255), cv::FILLED);
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}
/**
 * PlotPoint: Given a lat and a lon, plot the point on the map
 * 
 * @param  {double} lat : latitude
 * @param  {double} lon : longitude
 */
void TrojanMap::PlotPoint(double lat, double lon) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  auto result = GetPlotLocation(lat, lon);
  cv::circle(img, cv::Point(int(result.first), int(result.second)), DOT_SIZE,
             cv::Scalar(0, 0, 255), cv::FILLED);
  cv::startWindowThread();
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}

/**
 * PlotPath: Given a vector of location ids draws the path (connects the points)
 * 
 * @param  {std::vector<std::string>} location_ids : path
 */
void TrojanMap::PlotPath(std::vector<std::string> &location_ids) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  auto start = GetPlotLocation(data[location_ids[0]].lat, data[location_ids[0]].lon);
  cv::circle(img, cv::Point(int(start.first), int(start.second)), DOT_SIZE,
             cv::Scalar(0, 0, 255), cv::FILLED);
  for (auto i = 1; i < location_ids.size(); i++) {
    auto start = GetPlotLocation(data[location_ids[i - 1]].lat, data[location_ids[i - 1]].lon);
    auto end = GetPlotLocation(data[location_ids[i]].lat, data[location_ids[i]].lon);
    cv::circle(img, cv::Point(int(end.first), int(end.second)), DOT_SIZE,
               cv::Scalar(0, 0, 255), cv::FILLED);
    cv::line(img, cv::Point(int(start.first), int(start.second)),
             cv::Point(int(end.first), int(end.second)), cv::Scalar(0, 255, 0),
             LINE_WIDTH);
  }
  cv::startWindowThread();
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}

/**
 * PlotPoints: Given a vector of location ids draws the points on the map (no path).
 * 
 * @param  {std::vector<std::string>} location_ids : points
 */
void TrojanMap::PlotPoints(std::vector<std::string> &location_ids) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  for (auto x : location_ids) {
    auto result = GetPlotLocation(data[x].lat, data[x].lon);
    cv::circle(img, cv::Point(result.first, result.second), DOT_SIZE,
               cv::Scalar(0, 0, 255), cv::FILLED);
  }
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}


/**
 * CreateAnimation: Create the videos of the progress to get the path
 * 
 * @param  {std::vector<std::vector<std::string>>} path_progress : the progress to get the path
 */
void TrojanMap::CreateAnimation(std::vector<std::vector<std::string>> path_progress){
  cv::VideoWriter video("src/lib/output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(1248,992));
  for(auto location_ids: path_progress) {
    std::string image_path = cv::samples::findFile("src/lib/input.jpg");
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
    auto start = GetPlotLocation(data[location_ids[0]].lat, data[location_ids[0]].lon);
    cv::circle(img, cv::Point(int(start.first), int(start.second)), DOT_SIZE,
              cv::Scalar(0, 0, 255), cv::FILLED);
    for (auto i = 1; i < location_ids.size(); i++) {
      auto start = GetPlotLocation(data[location_ids[i - 1]].lat, data[location_ids[i - 1]].lon);
      auto end = GetPlotLocation(data[location_ids[i]].lat, data[location_ids[i]].lon);
      cv::circle(img, cv::Point(int(end.first), int(end.second)), DOT_SIZE,
                cv::Scalar(0, 0, 255), cv::FILLED);
      cv::line(img, cv::Point(int(start.first), int(start.second)),
              cv::Point(int(end.first), int(end.second)), cv::Scalar(0, 255, 0),
              LINE_WIDTH);
    }
    video.write(img);
    cv::imshow("TrojanMap", img);
    cv::waitKey(1);
  }
	video.release();
}
/**
 * GetPlotLocation: Transform the location to the position on the map
 * 
 * @param  {double} lat         : latitude 
 * @param  {double} lon         : longitude
 * @return {std::pair<double, double>}  : position on the map
 */
std::pair<double, double> TrojanMap::GetPlotLocation(double lat, double lon) {
  std::pair<double, double> bottomLeft(34.01001, -118.30000);
  std::pair<double, double> upperRight(34.03302, -118.26502);
  double h = upperRight.first - bottomLeft.first;
  double w = upperRight.second - bottomLeft.second;
  std::pair<double, double> result((lon - bottomLeft.second) / w * 1248,
                                   (1 - (lat - bottomLeft.first) / h) * 992);
  return result;
}

//-----------------------------------------------------
// TODO: Student should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id.
 * 
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(std::string id) { return data[id].lat; }

/**
 * GetLon: Get the longitude of a Node given its id. 
 * 
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(std::string id) { return data[id].lon; }

/**
 * GetName: Get the name of a Node given its id.
 * 
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(std::string id) { return data[id].name; }

/**
 * GetNeighborIDs: Get the neighbor ids of a Node.
 * 
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(std::string id) {
    std::vector<std::string> result = data[id].neighbors;
    return result;
}


/**
 * CalculateDistance: Get the distance between 2 nodes. 
 * 
 * @param  {Node} a  : node a
 * @param  {Node} b  : node b
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const Node &a, const Node &b) {
  // TODO: Use Haversine Formula:
  // dlon = lon2 - lon1;
  // dlat = lat2 - lat1;
  // a = (sin(dlat / 2)) ^ 2 + cos(lat1) * cos(lat2) * (sin(dlon / 2)) ^ 2;
  // c = 2 * arcsin(min(1, sqrt(a)));
  // distances = 3961 * c;

  // where 3961 is the approximate radius of the earth at the latitude of
  // Washington, D.C., in miles
  double lat1 = a.lat;
  double lat2 = b.lat;
  double lon1 = a.lon;
  double lon2 = b.lon;
  double dLat = (lat2 - lat1)*M_PI/180.0;
  double dLon = (lon2 - lon1)*M_PI/180.0;
  lat1 = (lat1)* M_PI / 180.0;
  lat2 = (lat2)* M_PI / 180.0;
  double aa = pow(sin(dLat/2),2) + pow(sin(dLon/2),2) * cos(lat1) * cos(lat2);
  double c = 2*asin(sqrt(aa));
  return 3961*c;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations inside the vector.
 * 
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  double sum = 0;
  for(int i = 0; i<path.size()-1;i++){
    sum = sum + CalculateDistance(data[path[i]],data[path[i+1]]);
  }
  return sum;
}

/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name) {
  std::vector<std::string> results;
  std::string subname;
  for(auto it:data){
    subname = it.second.name.substr(0,name.size());
    if(equal(name.begin(),name.end(),subname.begin(),
                                    [](const char& a, const char& b){
                                      return toupper(a) == toupper(b);
                                    }
                                    )){
      results.push_back(it.second.name);
    }
  }
  return results;
}


/**
 * GetPosition: Given a location name, return the position.
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  std::pair<double, double> results(-1, -1);
  for(auto it:data){
    if(it.second.name == name){
      results.first = it.second.lat;
      results.second = it.second.lon;
    }
  }
  return results;
}

/**
 * CalculateShortestPath: Given 2 locations, return the shortest path which is a
 * list of id.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath(
    std::string location1_name, std::string location2_name) {
  std::vector<std::string> x;
  std::map<std::string, double> d;
  std::map<std::string, std::vector<std::string>> path;
  std::string start;
  std::string goal;

  //initialize the distance map and the path map
  for (auto it : data){
    d[it.second.id] = double(INT16_MAX);
    if (location1_name == it.second.name){
      start = it.second.id;
      d[start] = 0;
      path[start].push_back(start);
    }
    if (location2_name == it.second.name){
      goal = it.second.id;
    }
  }
  // iteratively update the new distance map and path map
  // 1st loop: run for n times(n is the size of data)
  for (int i=0; i < data.size()-1; i++){
    // 2nd loop: visit all nodes in the data
    for (auto it : data){
      std::string current_id = it.second.id;
      // 3rd loop: visit all neighbors of current location
      for (auto neighbor_id : it.second.neighbors){
        double current_dist = CalculateDistance(data[current_id], data[neighbor_id]);
        if (current_dist + d[current_id] < d[neighbor_id]){
          //update d map
          d[neighbor_id] = current_dist + d[current_id];
          //update path map
          std::vector<std::string> new_path = path[current_id];
          new_path.push_back(neighbor_id);
          path[neighbor_id] = new_path;
        }
      }
    }
  }
  x = path[goal];
  return x;
}



/**
 * Travelling salesman problem based on Brute Force method: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan(
                                    std::vector<std::string> &location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> results;
  std::vector<std::string> startresult;
  std::vector<std::string> sublocation_ids;
  // initialize the value of minimum distance and index of the solution
  double minD = double(INT32_MAX);
  int minIndex = 0;

  // put the original route a first
  for(int i=1; i<location_ids.size();i++){
    sublocation_ids.push_back(location_ids[i]);
  }
  // use a recursive helper function to get all possible route
  TSPhelper1(sublocation_ids,results.second,startresult);

  // find the route which has the minimum path distance
  for(int i=0;i<results.second.size();i++){
    results.second[i].insert(results.second[i].begin(),location_ids[0]);
    results.second[i].push_back(location_ids[0]);
    if(minD>CalculatePathLength(results.second[i]) && results.second[i][0] == location_ids[0]){
      minD = CalculatePathLength(results.second[i]);
      minIndex = i;
    }
  }
  results.first = minD;
  results.second[results.second.size()-1].swap(results.second[minIndex]);
  return results;
} 

// use Brute Force method to generate all possible route
void TrojanMap::TSPhelper1(std::vector<std::string> &location_ids, 
                          std::vector<std::vector<std::string>> &result,
                          std::vector<std::string> curResult){
  // only store routes which is shorter than the location_ids
  if(curResult.size() == location_ids.size() && CalculatePathLength(curResult) < CalculatePathLength(location_ids)){
    result.push_back(curResult);
    return;
  }
  for(int i=0; i < location_ids.size();i++){
    if(find(curResult.begin(),curResult.end(),location_ids[i]) != curResult.end()){
      continue;
    }
    std::vector<std::string> nextResult = curResult;
    nextResult.push_back(location_ids[i]);
    if(CalculatePathLength(nextResult) < CalculatePathLength(location_ids)){
      TSPhelper1(location_ids,result,nextResult);
    }
  }
}

/**
 * Travelling salesman problem based on 2-opt method: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_2opt(
                                    std::vector<std::string> &location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> results;
  std::vector<std::string> curResult = location_ids;
  results.second.push_back(curResult);
  double minD = double(INT32_MAX);
  int minIndex = 0;
  TSPhelper2(results.second,location_ids,minD);
  for(int i=0;i<results.second.size();i++){
    results.second[i].push_back(location_ids[0]);
    if(minD>CalculatePathLength(results.second[i])){
      minD = CalculatePathLength(results.second[i]);
      minIndex = i;
    }
  }
  results.first = minD;
  results.second[results.second.size()-1].swap(results.second[minIndex]);


  return results;                            
}

// use 2-opt method to generate new route
void TrojanMap::TSPhelper2(std::vector<std::vector<std::string>> &result,
                          std::vector<std::string> curResult,
                          double minD){
  // filter the route which has larger distence than current minimum route
  if(minD<=CalculatePathLength(curResult)){
    return;
  }
  else{
    std::vector<std::string> nextResult;
    for(int i=1;i<curResult.size()-1;i++){
      for(int j=i+1;j<curResult.size();j++){
        nextResult = curResult;
        std::reverse(nextResult.begin()+i,nextResult.begin()+j+1);
        // only add the route which has smaller distence and update the minimum distance
        if(find(result.begin(),result.end(),nextResult) == result.end()
           && CalculatePathLength(curResult)>CalculatePathLength(nextResult)){
          result.push_back(nextResult);
          TSPhelper2(result,nextResult,CalculatePathLength(curResult));
        }
      }
    }
  }
}

/**
 * Travelling salesman problem based on 3-opt method: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_3opt(
                                    std::vector<std::string> &location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> results;
  std::vector<std::string> curResult = location_ids;
  
  results.second.push_back(curResult);
  double minD = double(INT32_MAX);
  int minIndex = 0;
  TSPhelper3(results.second,location_ids,minD);
  for(int i=0;i<results.second.size();i++){
    results.second[i].push_back(location_ids[0]);
    if(minD>CalculatePathLength(results.second[i])){
      minD = CalculatePathLength(results.second[i]);
      minIndex = i;
    }
  }
  results.first = minD;
  results.second[results.second.size()-1].swap(results.second[minIndex]);


  return results;                            
}

// use 3-opt method to generate new route
void TrojanMap::TSPhelper3(std::vector<std::vector<std::string>> &result,
                          std::vector<std::string> curResult,
                          double minD){
  // filter the route which has larger distence than current minimum route
  if(minD<=CalculatePathLength(curResult)){
    return;
  }
  else{
    std::vector<std::string> nextResult1;
    std::vector<std::string> nextResult2;
    std::vector<std::string> nextResult3;
    std::vector<std::string> nextResult4;
    std::vector<std::string> nextResult5;
    //std::vector<std::string> nextResult6;
    //std::vector<std::string> nextResult7;
    //std::vector<std::vector<std::string>> nextResults;
    //std::vector<std::string> bestNextResult;
    for(int i=1;i<curResult.size()-1;i++){
      for(int j=i+1;j<curResult.size();j++){
        std::vector<std::string> a;
        std::vector<std::string> b;
        std::vector<std::string> c;
        for(int it=0;it<i;it++){a.push_back(curResult[it]);}
        for(int it=i;it<j;it++){b.push_back(curResult[it]);}
        for(int it=j;it<curResult.size();it++){c.push_back(curResult[it]);}
        std::vector<std::string> br = b;
        std::reverse(br.begin(),br.end());
        std::vector<std::string> cr = c;
        std::reverse(cr.begin(),cr.end());
        // a-> b<- c<-
        for(int it=0;it<a.size();it++){nextResult1.push_back(a[it]);}
        for(int it=0;it<br.size();it++){nextResult1.push_back(br[it]);}
        for(int it=0;it<cr.size();it++){nextResult1.push_back(cr[it]);}
        
        if(find(result.begin(),result.end(),nextResult1) == result.end()
           && CalculatePathLength(curResult)>CalculatePathLength(nextResult1)){
          result.push_back(nextResult1);
          TSPhelper3(result,nextResult1,CalculatePathLength(curResult));
        }

        // a-> c-> b->
        for(int it=0;it<a.size();it++){nextResult2.push_back(a[it]);}
        for(int it=0;it<c.size();it++){nextResult2.push_back(c[it]);}
        for(int it=0;it<b.size();it++){nextResult2.push_back(b[it]);}
        
        if(find(result.begin(),result.end(),nextResult2) == result.end()
          && CalculatePathLength(curResult)>CalculatePathLength(nextResult2)){
          result.push_back(nextResult2);
          TSPhelper3(result,nextResult2,CalculatePathLength(curResult));
        }
        

        // a-> c<- b->
        for(int it=0;it<a.size();it++){ nextResult3.push_back(a[it]); }
        for(int it=0;it<cr.size();it++){ nextResult3.push_back(cr[it]); }
        for(int it=0;it<b.size();it++){ nextResult3.push_back(b[it]); }
        
        if(find(result.begin(),result.end(),nextResult3) == result.end()
           && CalculatePathLength(curResult)>CalculatePathLength(nextResult3)){
          result.push_back(nextResult3);
          TSPhelper3(result,nextResult3,CalculatePathLength(curResult));
        }

        // a-> c-> b<-
        for(int it=0;it<a.size();it++){ nextResult4.push_back(a[it]); }
        for(int it=0;it<c.size();it++){ nextResult4.push_back(c[it]); }
        for(int it=0;it<br.size();it++){ nextResult4.push_back(br[it]); }
        
        if(find(result.begin(),result.end(),nextResult4) == result.end()
           && CalculatePathLength(curResult)>CalculatePathLength(nextResult4)){
          result.push_back(nextResult4);
          TSPhelper3(result,nextResult4,CalculatePathLength(curResult));
        }

        // other opt swap which is the same as 2-opt 
        nextResult5 = curResult;
        std::reverse(nextResult5.begin()+i,nextResult5.begin()+j+1);
        if(find(result.begin(),result.end(),nextResult5) == result.end()
           && CalculatePathLength(curResult)>CalculatePathLength(nextResult5)){
          result.push_back(nextResult5);
          TSPhelper3(result,nextResult5,CalculatePathLength(curResult));
        }
      }
    }
  }
}
