#include "ObjectController.h"


ObjectController::ObjectController(const std::string& filepath) : m_Filepath(filepath) {
}

ObjectController::~ObjectController() {
	for (unsigned int i = 0; i < m_Objects.size(); i++) {
		delete m_Objects[i];
	}
}

void ObjectController::AddObject(ObjectType obj) {
	switch (obj) {
		case blank:
			m_Objects.push_back(new Object());
			break;
		case quad:
			m_Objects.push_back(new Quad());
			break;
	}
}

void ObjectController::RemoveObject(unsigned int index) {
	delete m_Objects[index];
	for (unsigned int i = index; i < (m_Objects.size() - 1); i++) {
		unsigned int next = i + 1;
		m_Objects[i] = m_Objects[next];
	}
}

void ObjectController::OnImGuiRender() {
}

void ObjectController::SaveObjects() {
}

void ObjectController::SaveObjectsAs(const std::string& filepath) {
	m_Filepath = filepath;
}

void ObjectController::LoadObjects(const std::string& filepath) {
	m_Filepath = filepath;
}

Object* ObjectController::GetObject(unsigned int index) { 
	return m_Objects[index]; 
}