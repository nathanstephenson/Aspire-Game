#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "Object.h"

class ObjectController : public Object {
private:
	std::vector<Object*> m_Objects;
	std::string m_Filepath;
public:
	ObjectController(const std::string& filepath);
	~ObjectController();

	void AddObject();
	void RemoveObject(unsigned int index);
	void OnImGuiRender() override;
	void SaveObjects();//to current file
	void SaveObjectsAs(const std::string& filepath);//to new file
	void LoadObjects(const std::string& filepath);//from file

	std::vector<Object*> GetObjects() { return m_Objects; }
};