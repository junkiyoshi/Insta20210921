#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void mouseEntered(int x, int y) {};
	void mouseExited(int x, int y) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	vector<glm::vec3> base_color_list;

	int number_of_targets;
	vector<glm::vec2> target_list;
	vector<glm::vec3> target_color_list;

	int frame_span;
	float size;
	vector<glm::vec2> location_list;

	vector<int> current_index_list;
	vector<int> next_index_list;
	vector<glm::vec3> color_list;

	ofShader shader;
};