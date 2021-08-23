#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 200, true, true, true);

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);

	string word = "LACK";
	this->font.drawString(word, this->font.stringWidth(word) * -0.56, this->font.stringHeight(word) - 250);

	fbo.end();

	int span = 5;
	ofPixels pixels;
	fbo.readToPixels(pixels);
	for (int x = 0; x < fbo.getWidth(); x += span) {

		for (int y = 0; y < fbo.getHeight(); y += span) {

			ofColor color = pixels.getColor(x, y);
			if (color != ofColor(0, 0)) {

					this->location_list.push_back(glm::vec3(x - fbo.getWidth() * 0.5, y - fbo.getHeight() * 0.5, 0));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	int span = 5;
	float threshold = 0.4;
	float frame_param = ofGetFrameNum() * 0.015;
	float coordinate_param = 0.0065;

	for (int i = 0; i < this->location_list.size(); i++) {

		for (int z = -100; z <= 100; z += 50) {

			auto x = this->location_list[i].x;
			auto y = this->location_list[i].y;

			auto noise_location = glm::vec3(x, y, z);
			auto noise_value = ofNoise(glm::vec3(noise_location.x * coordinate_param, noise_location.y * coordinate_param, noise_location.z * coordinate_param + frame_param));
			if (noise_value < threshold) { continue; }

			auto loc_1 = glm::vec3(x - span, y, z);
			auto noise_1 = this->foundLocation(loc_1) ? ofNoise(glm::vec3(loc_1.x * coordinate_param, loc_1.y * coordinate_param, loc_1.z * coordinate_param + frame_param)) : 0;
			auto loc_2 = glm::vec3(x, y + span, z);
			auto noise_2 = this->foundLocation(loc_2) ? ofNoise(glm::vec3(loc_2.x * coordinate_param, loc_2.y * coordinate_param, loc_2.z * coordinate_param + frame_param)) : 0;
			auto loc_3 = glm::vec3(x + span, y, z);
			auto noise_3 = this->foundLocation(loc_3) ? ofNoise(glm::vec3(loc_3.x * coordinate_param, loc_3.y * coordinate_param, loc_3.z * coordinate_param + frame_param)) : 0;
			auto loc_4 = glm::vec3(x, y - span, z);
			auto noise_4 = this->foundLocation(loc_4) ? ofNoise(glm::vec3(loc_4.x * coordinate_param, loc_4.y * coordinate_param, loc_4.z * coordinate_param + frame_param)) : 0;

			auto index = this->face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(x - span * 0.5, y - span * 0.5, z));
			vertices.push_back(glm::vec3(x + span * 0.5, y - span * 0.5, z));
			vertices.push_back(glm::vec3(x + span * 0.5, y + span * 0.5, z));
			vertices.push_back(glm::vec3(x - span * 0.5, y + span * 0.5, z));

			this->face.addVertices(vertices);

			this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 2);
			this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

			if (noise_1 < threshold) {

				this->frame.addVertex(vertices[0]);
				this->frame.addVertex(vertices[3]);

				this->frame.addIndex(this->frame.getNumVertices() - 1);
				this->frame.addIndex(this->frame.getNumVertices() - 2);
			}

			if (noise_2 < threshold) {

				this->frame.addVertex(vertices[2]);
				this->frame.addVertex(vertices[3]);

				this->frame.addIndex(this->frame.getNumVertices() - 1);
				this->frame.addIndex(this->frame.getNumVertices() - 2);
			}

			if (noise_3 < threshold) {

				this->frame.addVertex(vertices[1]);
				this->frame.addVertex(vertices[2]);

				this->frame.addIndex(this->frame.getNumVertices() - 1);
				this->frame.addIndex(this->frame.getNumVertices() - 2);
			}

			if (noise_4 < threshold) {

				this->frame.addVertex(vertices[0]);
				this->frame.addVertex(vertices[1]);

				this->frame.addIndex(this->frame.getNumVertices() - 1);
				this->frame.addIndex(this->frame.getNumVertices() - 2);

			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	ofSetColor(239);
	this->frame.draw();

	ofSetColor(39);
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
bool ofApp::foundLocation(glm::vec3 value) {

	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->location_list[i] == glm::vec3(value.x, value.y, 0)) {

			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}