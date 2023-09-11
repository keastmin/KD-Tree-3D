#include "KdTree.h"

KdNode::KdNode() {
	for (int i = 0; i < 2; i++) {
		_childNodes[i] = nullptr;
	}
}


KdNode::~KdNode() {

}

void KdNode::draw() {
	//_min.Print();
	//_max.Print();
	//cout << endl;

	glBegin(GL_LINES);

	//glVertex3f(-0.050000, -0.050000, -0.050000);
	//glVertex3f(-0.050000, 1.050000, -0.050000);
	//glVertex3f(-0.050000, 1.050000, -0.050000);
	//glVertex3f(1.050000, 1.050000, -0.050000);
	//glVertex3f(1.050000, 1.050000, -0.050000);
	//glVertex3f(1.050000, -0.050000, -0.050000);
	//glVertex3f(1.050000, -0.050000, -0.050000);
	//glVertex3f(-0.050000, -0.050000, -0.050000);

	//glVertex3f(-0.050000, -0.050000, -0.050000);
	//glVertex3f(-0.050000, -0.050000, 1.050000);
	//glVertex3f(-0.050000, 1.050000, -0.050000);
	//glVertex3f(-0.050000, 1.050000, 1.050000);
	//glVertex3f(1.050000, 1.050000, -0.050000);
	//glVertex3f(1.050000, 1.050000, 1.050000);
	//glVertex3f(1.050000, -0.050000, -0.050000);
	//glVertex3f(1.050000, -0.050000, 1.050000);

	//glVertex3f(-0.050000, -0.050000, 1.050000);
	//glVertex3f(-0.050000, 1.050000, 1.050000);
	//glVertex3f(-0.050000, 1.050000, 1.050000);
	//glVertex3f(1.050000, 1.050000, 1.050000);
	//glVertex3f(1.050000, 1.050000, 1.050000);
	//glVertex3f(1.050000, -0.050000, 1.050000);
	//glVertex3f(1.050000, -0.050000, 1.050000);
	//glVertex3f(-0.050000, -0.050000, 1.050000);


	glVertex3f(_min.x(), _min.y(), _min.z());
	glVertex3f(_min.x(), _max.y(), _min.z());
	glVertex3f(_min.x(), _max.y(), _min.z());
	glVertex3f(_max.x(), _max.y(), _min.z());
	glVertex3f(_max.x(), _max.y(), _min.z());
	glVertex3f(_max.x(), _min.y(), _min.z());
	glVertex3f(_max.x(), _min.y(), _min.z());
	glVertex3f(_min.x(), _min.y(), _min.z());

	glVertex3f(_min.x(), _min.y(), _min.z());
	glVertex3f(_min.x(), _min.y(), _max.z());
	glVertex3f(_min.x(), _max.y(), _min.z());
	glVertex3f(_min.x(), _max.y(), _max.z());
	glVertex3f(_max.x(), _max.y(), _min.z());
	glVertex3f(_max.x(), _max.y(), _max.z());
	glVertex3f(_max.x(), _min.y(), _min.z());
	glVertex3f(_max.x(), _min.y(), _max.z());

	glVertex3f(_min.x(), _min.y(), _max.z());
	glVertex3f(_min.x(), _max.y(), _max.z());
	glVertex3f(_min.x(), _max.y(), _max.z());
	glVertex3f(_max.x(), _max.y(), _max.z());
	glVertex3f(_max.x(), _max.y(), _max.z());
	glVertex3f(_max.x(), _min.y(), _max.z());
	glVertex3f(_max.x(), _min.y(), _max.z());
	glVertex3f(_min.x(), _min.y(), _max.z());

	glEnd();
}

bool KdNode::IsContain(Vec3<double> p)
{
	if (p.x() >= _min.x() && p.x() <= _max.x() && p.y() >= _min.y() && p.y() <= _max.y() && p.z() >= _min.z() && p.z() <= _max.z()) {
		return true;
	}
	return false;
}

bool KdNode::IsIntersect(Vec3<double> p, double r)
{
	double dist = 0.0;
	for (int i = 0; i < 3; i++) {
		if (p(i) < _min(i)) {
			double d = _min(i) - p(i);
			dist += d * d;
		}
		else if (p(i) > _max(i)) {
			double d = p(i) - _max(i);
			dist += d * d;
		}
	}
	return dist <= r * r;
}

bool KdNode::IsEmpty(void)
{
	return _childNodes[0] == nullptr;
}

bool KdNode::IsContain(Vec3<double> p, double r)
{
	double minx = p.x() - r;
	double maxx = p.x() + r;
	double miny = p.y() - r;
	double maxy = p.y() + r;
	double minz = p.z() - r;
	double maxz = p.z() + r;
	return (minx >= _min.x() && maxx < _max.x() && miny >= _min.y() && maxy < _max.y() && minz >= _min.z() && maxz < _max.z());
}

void KdNode::subdivide() {
	for (int i = 0; i < 2; i++) {
		_childNodes[i] = new KdNode();
	}

	auto crossVec = _max - _min;
	int splitAxis;
	if (crossVec.x() > crossVec.y())
		splitAxis = crossVec.x() > crossVec.z() ? 0 : 2;
	else
		splitAxis = crossVec.y() > crossVec.z() ? 1 : 2;
	cout << splitAxis << endl;
	int mid = _particles.size() / 2;
	nth_element(_particles.begin(), _particles.begin() + mid, _particles.end(), [splitAxis](Particle* a, Particle* b) {return a->_pos[splitAxis] < b->_pos[splitAxis]; });

	auto splitPos = _particles[mid]->_pos[splitAxis];
	auto localMin = _min;
	auto localMax = _max;

	localMin[splitAxis] = splitPos;
	localMax[splitAxis] = splitPos;

	_childNodes[0]->_min = _min;
	_childNodes[0]->_max = localMax;
	_childNodes[1]->_min = localMin;
	_childNodes[1]->_max = _max;

	_childNodes[0]->_particles.clear();
	_childNodes[1]->_particles.clear();
	_childNodes[0]->_particles.assign(_particles.begin(), _particles.begin() + mid);
	_childNodes[1]->_particles.assign(_particles.begin() + mid, _particles.end());
	_particles.clear();
}

KdTree::KdTree() {

}

KdTree::~KdTree() {

}

void KdTree::buildTree(vector<Particle*>& particles) {
	double offset = 0.05;
	Vec3<double> min(0.0, 0.0, 0.0);
	Vec3<double> max(1.0, 1.0, 1.0);

	_root = new KdNode(min - offset, max + offset);

	for (auto p : particles) {
		_root->_particles.push_back(p);
	}

	vector<KdNode*> queue;
	queue.push_back(_root);
	auto node = queue[0];
	//cout<<node->_particles.size();

	while (queue.size() != 0) {
		auto node = queue[0];
		_renderTree.push_back(node);
		queue.erase(queue.begin());
		if (node->_particles.size() > _bucket) {
			node->subdivide();
			for (int i = 0; i < 2; i++) {
				queue.push_back(node->_childNodes[i]);
			}
		}
	}
	cout << _renderTree.size();
}

void KdTree::query(vector<Particle*>& particles, double r, int query_index) {
	auto pos = particles[query_index]->_pos;

	vector<KdNode*> queue;
	queue.push_back(_root);

	while (queue.size() != 0) {
		auto node = queue[0];
		queue.erase(queue.begin());
		bool isContain = node->IsContain(pos, r);
		bool isIntersect = node->IsIntersect(pos, r);
		if (isContain == true || isIntersect == true) {
			for (auto p : node->_particles) {
				p->_active = true;
				double sdf = pow(p->_pos.x() - pos.x(), 2.0) + pow(p->_pos.y() - pos.y(), 2.0) + pow(p->_pos.z() - pos.z(), 2.0) - r * r;
				if (sdf <= 0.0) {
					p->_collision = true;
				}
			}
			if (!node->IsEmpty()) {
				for (int i = 0; i < 2; i++) {
					queue.push_back(node->_childNodes[i]);
				}
			}
		}
	}
}

void KdTree::draw() {
	glLineWidth(1.0f);
	glDisable(GL_LIGHTING);
	for (auto n : _renderTree) {
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		n->draw();
		//glPopMatrix();
	}
	glEnable(GL_LIGHTING);
}