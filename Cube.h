#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include <iostream>

class Cube : public Shape {
public:
	// 6 by (xseg + 1)*(yseg + 1)2d vectors; each row represents a face
	vector< vector<vertex> > vertex_list_list;
	vector<triangle> triangle_list;
	int num_faces = 6;

	Cube() {
		setupShape();
	};
	~Cube() {};

	OBJ_TYPE getType() {
		return SHAPE_CUBE;
	}

	void setupShape() {
		// Resetting lists of vertices and triangles each time
		for (int v_list = 0; v_list < vertex_list_list.size(); v_list++) {
			vertex_list_list[v_list].clear();
		}
		vertex_list_list.clear();
		triangle_list.clear();

		// "Front" face: X-Y
		float start_x = -0.5;
		float start_y = 0.5;
		float start_z = 0.5;
		gen_vertices_faces(start_x, start_y, start_z, Z);

		// "Top" face: X-Z
		start_z = -0.5;
		gen_vertices_faces(start_x, start_y, start_z, Y);

		// "Right" face: Z-Y
		start_x = 0.5;
		start_z = 0.5;
		gen_vertices_faces(start_x, start_y, start_z, X);
	}


	// @TODO: don't need back_v?
	void gen_vertices_faces(float start_x, float start_y, float start_z, int const_dim) {
		vector<vertex> front_vertex_list;
		vector<vertex> back_vertex_list;
		if (const_dim == X) {
			int z_dir = (start_z < 0) ? 1 : -1;
			int y_dir = (start_y < 0) ? 1 : -1;
			for (int y = 0; y < m_segmentsY + 1; y++) {
				for (int z = 0; z < m_segmentsX + 1; z++) {
					vertex front_v;
					vertex back_v;
					front_v.z = start_z + z_dir * z * size / m_segmentsX;
					front_v.y = start_y + y_dir * y * size / m_segmentsY;
					front_v.x = start_x;
					front_vertex_list.push_back(front_v);

					/*back_v.z = front_v.z;
					back_v.y = -front_v.y;
					back_v.x = -start_x;*/
					front_v.z = front_v.z;
					front_v.y = -front_v.y;
					front_v.x = -start_x;

					back_vertex_list.push_back(front_v);
				}
			}
		}
		else if (const_dim == Y) {
			int x_dir = (start_x < 0) ? 1 : -1;
			int z_dir = (start_z < 0) ? 1 : -1;
			for (int z = 0; z < m_segmentsY + 1; z++) {
				for (int x = 0; x < m_segmentsX + 1; x++) {
					vertex front_v;
					vertex back_v;
					front_v.x = start_x + x_dir * x * size / m_segmentsX;
					front_v.z = start_z + z_dir * z * size / m_segmentsY;
					front_v.y = start_y;
					front_vertex_list.push_back(front_v);

					back_v.x = front_v.x;
					back_v.z = -front_v.z;
					back_v.y = -start_y;
					back_vertex_list.push_back(back_v);
				}
			}
		}
		else if (const_dim == Z) {
			int x_dir = (start_x < 0) ? 1 : -1;
			int y_dir = (start_y < 0) ? 1 : -1;
			for (int y = 0; y < m_segmentsY + 1; y++) {
				for (int x = 0; x < m_segmentsX + 1; x++) {
					vertex front_v;
					vertex back_v;
					front_v.x = start_x + x_dir * x * size / m_segmentsX;
					front_v.y = start_y + y_dir * y * size / m_segmentsY;
					front_v.z = start_z;
					front_vertex_list.push_back(front_v);

					back_v.x = front_v.x;
					back_v.y = -front_v.y;
					back_v.z = -start_z;
					back_vertex_list.push_back(back_v);
				}
			}
		}

		for (int j = 0; j < m_segmentsY; j++) {
			for (int i = 0; i < m_segmentsX; i++) {
				// "Top" triangle
				triangle f1;
				f1.v_idx_list[0] = i + j * (m_segmentsX + 1);
				f1.v_idx_list[1] = f1.v_idx_list[0] + m_segmentsX + 1;
				f1.v_idx_list[2] = f1.v_idx_list[0] + 1;
				triangle_list.push_back(f1);

				// "Bottom" triangle
				triangle f2;
				f2.v_idx_list[0] = f1.v_idx_list[2];
				f2.v_idx_list[1] = f2.v_idx_list[0] + m_segmentsX;
				f2.v_idx_list[2] = f2.v_idx_list[0] + m_segmentsX + 1;
				triangle_list.push_back(f2);
			}
		}

		vertex_list_list.push_back(front_vertex_list);
		vertex_list_list.push_back(back_vertex_list);
	}

	void draw() {
		if (vertex_list_list.size() == 0 || triangle_list.size() == 0) {
			return;
		}

		int num_tris = triangle_list.size();
		int v_count = 3; // triangle_list[0].v_count

		num_faces = vertex_list_list.size();

		for (int face = 0; face < num_faces; face++) {
			float xs[3];
			float ys[3];
			float zs[3];

			for (int tri = 0; tri < num_tris; tri++) {
				for (int v = 0; v < v_count; v++) {
					xs[v] = vertex_list_list[face][triangle_list[tri].v_idx_list[v]].x;
					ys[v] = vertex_list_list[face][triangle_list[tri].v_idx_list[v]].y;
					zs[v] = vertex_list_list[face][triangle_list[tri].v_idx_list[v]].z;
				}
				glBegin(GL_TRIANGLES);
				setNormal(xs[0], ys[0], zs[0], xs[1], ys[1], zs[1], xs[2], ys[2], zs[2]);
				glVertex3f(xs[0], ys[0], zs[0]);
				glVertex3f(xs[1], ys[1], zs[1]);
				glVertex3f(xs[2], ys[2], zs[2]);
				glEnd();
			}
		}
	};


	void drawNormal() {
		if (vertex_list_list.size() == 0 || triangle_list.size() == 0) {
			return;
		}

		num_faces = vertex_list_list.size();
		int num_points = vertex_list_list[0].size();
		bool positive = true;
		for (int face = 0; face < num_faces; face++) {

			float normal[3] = { 0,0,0 };
			if (face % 2 == 0)
				positive = true;
			else
				positive = false;

			int const_dimen = 2 - face / 2;

			if (positive)
				normal[const_dimen] = 0.1;
			else
				normal[const_dimen] = -0.1;

			for (int i = 0; i < num_points; i++) {
				vertex v = vertex_list_list[face][i];
				glBegin(GL_LINES);
				glVertex3f(v.x, v.y, v.z); glVertex3f(normal[0] + v.x, normal[1] + v.y, normal[2] + v.z);
				glEnd();
			}
		}
	};

private:
};

#endif