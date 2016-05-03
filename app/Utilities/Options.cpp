#include "Options.h"

#include <osgGA\GUIEventHandler>

using namespace std;

const std::string Options::OPT_FORWARD = "forward";
const std::string Options::OPT_BACKWARD = "backward";
const std::string Options::OPT_LEFT = "left";
const std::string Options::OPT_RIGHT = "right";
const std::string Options::OPT_JUMP = "jump";
const std::string Options::OPT_FULLSCREEN_KEY = "fullscreen_key";
const std::string Options::OPT_FULLSCREEN = "fullscreen";
const std::string Options::OPT_SCREEN_WIDTH = "screen_width";
const std::string Options::OPT_SCREEN_HEIGHT = "screen_height";
const std::string Options::OPT_SCREEN_NUM = "screen_num";
const std::string Options::OPT_FOV = "field_of_view";

Options::Options(string name) {
	load(name);
	fillDefault();
}

string Options::get(string name) {
    if(settings.find(name)!=settings.end())
        return settings[name];
    else if(defaultSettings.find(name)!=defaultSettings.end())
        return defaultSettings[name];
    else
        throw runtime_error("Could not find option!");
}

void Options::load(string name) {
    ifstream file;
	fileName = name;
	file.open(name.c_str(), ifstream::in); //Opens file

	if(!file.good()) { //does it exist?
		file.close(); //Closes it so we don't mess with creating a new options file
		cerr << "Options file doesn't exist, creating it" << endl;
		reset();
		file.open(name.c_str()); //Opens file
	}

	string line;
	stringstream ss;
	string linePart;
	string arg;

	//Reads in all the arguments
	while(!file.eof()) {
		stringstream ss;
		getline(file, line);
		ss << line.c_str();
		//Option
		getline(ss, linePart, '=');
		linePart.erase( remove_if( linePart.begin(), linePart.end(), ::isspace ), linePart.end() ); //Removes spaces
		//Argument
		getline(ss, arg);
		arg.erase( remove_if( arg.begin(), arg.end(), ::isspace ), arg.end() ); //removes spaces
		if(linePart.size() >= 1)
			settings[linePart]=arg;
	}
	file.close();
}

void Options::fillDefault() {
    defaultSettings[OPT_FORWARD]=to_string(osgGA::GUIEventAdapter::KeySymbol::KEY_W);
    defaultSettings[OPT_LEFT]=to_string(osgGA::GUIEventAdapter::KeySymbol::KEY_A);
    defaultSettings[OPT_BACKWARD]=to_string(osgGA::GUIEventAdapter::KeySymbol::KEY_S);
    defaultSettings[OPT_RIGHT]=to_string(osgGA::GUIEventAdapter::KeySymbol::KEY_D);
    defaultSettings[OPT_JUMP]=to_string(osgGA::GUIEventAdapter::KeySymbol::KEY_Space);
	defaultSettings[OPT_FULLSCREEN_KEY] = to_string(osgGA::GUIEventAdapter::KeySymbol::KEY_F11);
    defaultSettings[OPT_FULLSCREEN]="true";
    defaultSettings[OPT_SCREEN_WIDTH]="640";
    defaultSettings[OPT_SCREEN_HEIGHT]="480";
	defaultSettings[OPT_SCREEN_NUM] = "0";
	defaultSettings[OPT_FOV] = "60";
    defaultSettings["screen_bbp"]="32";
    defaultSettings["verbose"]="false";
    defaultSettings["maxFPS"]="60";
    defaultSettings["pixels_per_meter"]="32";
    defaultSettings["physics_debug"]="false";
    defaultSettings["velocity_iterations"]="8";
    defaultSettings["position_iterations"]="3";
    defaultSettings["physics_subsets"]="1";
}

void Options::save() {
	ofstream file;
	file.open(fileName.c_str());
	for (unordered_map<string,string>::iterator it=settings.begin(); it!=settings.end(); ++it)
    	file << it->first << "=" << it->second << endl;

    //Fill file with any options not in settings (new settings, typically)
    for (unordered_map<string,string>::iterator it=defaultSettings.begin(); it!=defaultSettings.end(); ++it)
    {
        if(settings.find(it->first)==settings.end())
            file << it->first << "=" << it->second << endl;
    }
	file.close();
}


void Options::debugOut() {
	for (unordered_map<string,string>::iterator it=settings.begin(); it!=settings.end(); ++it)
    	cout << it->first << "=" << it->second << endl;
}

void Options::reset() {
	ofstream file;
	file.open(fileName.c_str());
	if(!file.is_open())
		cout << "Failed to open option file for writing" << endl;
    for (unordered_map<string,string>::iterator it=defaultSettings.begin(); it!=defaultSettings.end(); ++it)
    	file << it->first << "=" << it->second << endl;
	file.close();
	file.close();
}
