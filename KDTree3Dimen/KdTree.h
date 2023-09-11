#pragma once
#include <iostream>
#include <algorithm>
#include "Vec3.h"
#include "Particle.h"
#include "GL/glut.h"
#include <vector>

using namespace std;

class KdNode{
public:
	Vec3<double> _min;
	Vec3<double> _max;
	vector<Particle*> _particles;
	KdNode* _childNodes[2];
public:
	KdNode();
	KdNode(Vec3<double> min, Vec3<double> max) {
		_min = min; 
		_max = max;
		for (int i = 0; i < 2; i++) {
			_childNodes[i] = nullptr;
		}
	}
	~KdNode();
public:
	bool IsContain(Vec3<double> p);
	bool IsIntersect(Vec3<double> p, double r);
	bool IsEmpty(void);
	bool IsContain(Vec3<double> p, double r);
	void subdivide();
public:
	void draw();
};

class KdTree
{
public:
	KdNode* _root;
	int _bucket;
	vector<KdNode*> _renderTree;
public:
	KdTree();
	KdTree(int bucket) {
		_bucket = bucket;
	}
	~KdTree();
public:
	void buildTree(vector<Particle*>& particles);
	void query(vector<Particle*>& particles, double r, int query_index);
public:
	void draw();
};

