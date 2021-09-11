#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>
//object types
#include "Quad.h"

class ObjectController : public Object {
public:
	ObjectController(const std::string& filepath);
	~ObjectController();

	void AddObject(ObjectType obj);
	void RemoveObject(unsigned int index);
	void OnImGuiRender() override;
	void SaveObjects();//to current file
	void SaveObjectsAs(const std::string& filepath);//to new file
	void LoadObjects(const std::string& filepath);//from file

	std::vector<Object*> GetObjects() { return m_Objects; }
private:
	std::vector<Object*> m_Objects;
	std::string m_Filepath;
};