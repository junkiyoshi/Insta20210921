#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetCircleResolution(72);

	this->base_color_list.push_back(glm::vec3(0.4, 0.25, 0.25));
	this->base_color_list.push_back(glm::vec3(0.25, 0.4, 0.25));
	this->base_color_list.push_back(glm::vec3(0.25, 0.25, 0.4));

	this->frame_span = 60;
	this->size = 15;
	this->number_of_targets = 120;

	for (int x = this->size * 6; x <= ofGetWidth() - this->size * 6; x += this->size * 3) {

		for (int y = this->size * 6; y <= ofGetHeight() - this->size * 6; y += this->size * 3) {

			this->location_list.push_back(glm::vec2(x, y));
		}
	}

	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->target_color_list.push_back(glm::vec3(1, 1, 1));

		this->current_index_list.push_back(0);
		this->next_index_list.push_back(ofRandom(this->location_list.size()));
		this->color_list.push_back(this->base_color_list[ofRandom(this->base_color_list.size())]);
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->current_index_list.size(); i++) {

		if (ofGetFrameNum() % (this->frame_span * 2) == 0) {

			int next_index = this->next_index_list[i];
			int current_index = next_index;

			vector<int> near_indexes;
			for (int location_index = 0; location_index < this->location_list.size(); location_index++) {

				if (current_index == location_index) { continue; }
				if (glm::distance(this->location_list[current_index], this->location_list[location_index]) < this->size * 5) {

					near_indexes.push_back(location_index);
				}
			}

			if (near_indexes.size() > 0) {

				next_index = near_indexes[ofRandom(near_indexes.size())];
			}

			this->current_index_list[i] = current_index;
			this->next_index_list[i] = next_index;
		}
	}

	for (int i = 0; i < this->number_of_targets; i++) {

		if (ofGetFrameNum() % (this->frame_span * 2) < this->frame_span) {

			this->target_list[i] = this->location_list[this->current_index_list[i]];
			this->target_color_list[i] = this->color_list[i];
		}
		else {

			this->target_list[i] = this->location_list[this->next_index_list[i]];
			this->target_color_list[i] = this->color_list[i];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	int frame_param = ofGetFrameNum() % this->frame_span;

	ofSetLineWidth(5);
	for (int i = 0; i < this->next_index_list.size(); i++) {

		ofSetColor(ofMap(this->target_color_list[i].x, 0.25, 0.4, 0, 255), ofMap(this->target_color_list[i].y, 0.25, 0.4, 0, 255), ofMap(this->target_color_list[i].z, 0.25, 0.4, 0, 255));

		int current_index = this->current_index_list[i];
		int next_index = this->next_index_list[i];

		auto angle_current = std::atan2(this->location_list[next_index].y - this->location_list[current_index].y, this->location_list[next_index].x - this->location_list[current_index].x);
		auto satellite_point_current = this->location_list[current_index] + glm::vec2(this->size * cos(angle_current), this->size * sin(angle_current));
		auto angle_next = std::atan2(this->location_list[current_index].y - this->location_list[next_index].y, this->location_list[current_index].x - this->location_list[next_index].x);
		auto satellite_point_next = this->location_list[next_index] + glm::vec2(this->size * cos(angle_next), this->size * sin(angle_next));

		if (ofGetFrameNum() % (this->frame_span * 2) < this->frame_span) {

			auto distance = glm::distance(satellite_point_next, satellite_point_current);
			distance = ofMap(frame_param, 0, this->frame_span, 0, distance);
			auto direction = satellite_point_next - satellite_point_current;
			ofDrawLine(satellite_point_current, satellite_point_current + glm::normalize(direction) * distance);
		}
		else {

			auto distance = glm::distance(satellite_point_next, satellite_point_current);
			distance = ofMap(frame_param, 0, this->frame_span, distance, 0);
			auto direction = satellite_point_next - satellite_point_current;
			ofDrawLine(satellite_point_next, satellite_point_next - glm::normalize(direction) * distance);
		}
	}

	ofSetLineWidth(0.1);
	ofSetColor(255);
	ofNoFill();
	for (int location_index = 0; location_index < this->location_list.size(); location_index++) {

		ofDrawCircle(this->location_list[location_index], this->size);
	}

	for (int i = 0; i < this->target_color_list.size(); i++) {

		if (ofGetFrameNum() % (this->frame_span * 2) < this->frame_span) {

			float p = ofGetFrameNum() % this->frame_span < this->frame_span * 0.5 ? 1 : ofMap(ofGetFrameNum() % this->frame_span, this->frame_span * 0.5, this->frame_span, 1, 0);
			this->target_color_list[i] = this->target_color_list[i] * p;
		}
		else {

			this->target_color_list[i] = this->target_color_list[i] * ofMap(ofGetFrameNum() % this->frame_span, 0, this->frame_span, 0, 1);
		}
	}

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->target_color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}