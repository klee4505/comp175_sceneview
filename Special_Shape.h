#ifndef SWORD_H
#define SWORD_H

#include "Shape.h"

class Sword : public Shape {
public:
	vector< vector<vertex> > vertex_list_list;
	vector< vector<triangle> > triangle_list_list;

	float cone_size = 0.1;
	float blade_width = 0.1;
	float blade_length = 0.6;

	float guard_shift;
	float pommel_shift;
	Sword() {
		setupShape();
	};
	~Sword() {};

	OBJ_TYPE getType() {
		return SHAPE_SPECIAL1;
	}

	void setupShape() {
		// Resetting lists of vertices and triangles each time
		for (int v_list = 0; v_list < vertex_list_list.size(); v_list++) {
			vertex_list_list[v_list].clear();
		}
		for (int t_list = 0; t_list < triangle_list_list.size(); t_list++) {
			triangle_list_list[t_list].clear();
		}
		vertex_list_list.clear();
		triangle_list_list.clear();

		gen_vertices_faces();
	}

	void gen_vertices_faces() {
		//Sword point
		float start_x = 0.5;
		float start_y = 0.3;
		float start_z = 0;
		float theta = 2 * PI / m_segmentsX;
		vector<vertex> cone_bot_vertex_list;
		vector<vertex> cone_side_vertex_list;
		vertex v;
		for (int x = 0; x < m_segmentsX; x++) {
			// Bot -- Note: builds in clock-wise direction
			v.x = cone_size / 2 * cos(x * theta);
			v.z = cone_size / 2 * sin(x * theta);
			v.y = start_y;
			cone_bot_vertex_list.push_back(v);

			//Side
			v = cone_bot_vertex_list[x];
			cone_side_vertex_list.push_back(v);
			// Wrap-around: sides collectively treated like a wrapped-up rectangle
			/* Order of filling:
			   Eg. for m_segmentsX = 3, m_segmentsY = 4:
				[[(4) 14 9 4]
				 [(3) 13 8 3]
				 [(2) 12 7 2]
				 [(1) 11 6 1]
				 [(0) 10 5 0]]
			*/
			start_x = v.x;
			start_z = v.z;
			for (int y = 1; y < m_segmentsY + 1; y++) {
				v.y = start_y + y * blade_width / m_segmentsY;
				v.x = start_x - y * start_x / m_segmentsY;
				v.z = start_z - y * start_z / m_segmentsY;
				cone_side_vertex_list.push_back(v);
			}
			//@TODO: final point in side_vertex_list should be (0 0.5 0) every time, check that
		}

		// bot_vertex_list[m_segmentsX] = center of bottom
		v.x = 0;
		v.y = start_y;
		v.z = 0;
		cone_bot_vertex_list.push_back(v);

		vertex_list_list.push_back(cone_side_vertex_list);


		//Blade
		start_x = 0.1;
		start_y = 0.3;
		start_z = 0;
		theta = 2 * PI / m_segmentsX;
		//vector<vertex> blade_top_vertex_list = cone_bot_vertex_list;
		vector<vertex> blade_side_vertex_list;
		vector<vertex> blade_bot_vertex_list;

		for (int x = 0; x < m_segmentsX; x++) {
			// Top -- Note: builds in clock-wise direction
			
			v = cone_bot_vertex_list[x];
			// Bottom
			v.y = start_y - blade_length;
			blade_bot_vertex_list.push_back(v);

			//Side
			v = cone_bot_vertex_list[x];
			blade_side_vertex_list.push_back(v);
			// Wrap-around: sides collectively treated like a wrapped-up rectangle
			/* Order of filling: 
			   Eg. for m_segmentsX = 3, m_segmentsY = 4:
				[[(0) 10 5 0]
				 [(1) 11 6 1]
				 [(2) 12 7 2]
				 [(3) 13 8 3]
				 [(4) 14 9 4]]
			*/
			for (int y = 1; y < m_segmentsY + 1; y++) {
				v.y = start_y - y * blade_length / m_segmentsY;
				blade_side_vertex_list.push_back(v);
			}
		}

		//v.y = start_y;
		//blade_bot_vertex_list.push_back(v);

//		vertex_list_list.push_back(bot_vertex_list);
		vertex_list_list.push_back(blade_side_vertex_list);

		//Guard
		float guard_height = 0.05;
		float guard_width = 0.2;
		guard_shift = start_y - blade_length - guard_height / 2;
		start_x = 0;
		start_y = guard_height / 2;
		start_z = 0;
		float thetaX = 2 * PI / m_segmentsX;
		float phiY = PI / m_segmentsY;
		vector<vertex> guard_top_vertex_list = blade_bot_vertex_list;
		vector<vertex> guard_bot_vertex_list;
		vector<vertex> guard_side_vertex_list;
		vector<float> layer_rads;

		for (int y = 0; y < m_segmentsY + 1; y++) {
			layer_rads.push_back(sin(phiY * y) * guard_width / 2);
		}

		for (int x = 0; x < m_segmentsX; x++) {
			// Bot -- Note: builds in clock-wise direction
			v.x = layer_rads[1] * cos(x * thetaX);
			v.z = layer_rads[1] * sin(x * thetaX);
			v.y = guard_height / 2 * cos(phiY * (m_segmentsY - 1)) + guard_shift;
			guard_bot_vertex_list.push_back(v);

			//Side
//			v = guard_bot_vertex_list[x];
			guard_side_vertex_list.push_back(v);

			//Goes from 2nd layer to last layer (first layer done by bot, last layer done for top)
			for (int y = 2; y < m_segmentsY; y++) {
				v.y = guard_height / 2 * cos(phiY * (m_segmentsY - y)) + guard_shift;
				v.x = layer_rads[y] * cos(x * thetaX);
				v.z = layer_rads[y] * sin(x * thetaX);
				guard_side_vertex_list.push_back(v);
			}
			//Add the top-most vertex to the guard_top_vertex_list
			guard_top_vertex_list.push_back(v);
			// Wrap-around: sides collectively treated like a wrapped-up rectangle
			/* Order of filling:
			   Eg. for m_segmentsX = 3, m_segmentsY = 4:
				[[(4) 14 9 4]
				 [(3) 13 8 3]
				 [(2) 12 7 2]
				 [(1) 11 6 1]
				 [(0) 10 5 0]]
			*/
		}
		v.x = start_x;
		v.y = start_y + guard_shift;
		v.z = start_z;
		guard_bot_vertex_list.push_back(v);
		guard_top_vertex_list.push_back(guard_top_vertex_list[m_segmentsX]);

		vertex_list_list.push_back(guard_top_vertex_list);
		vertex_list_list.push_back(guard_side_vertex_list);


		//grip
		float grip_length = 0.15;
		float grip_width = 0.1;
		start_x = 0.1;
		start_y += guard_shift;
		start_z = 0;
		theta = 2 * PI / m_segmentsX;
		//vector<vertex> grip_top_vertex_list = guard_bot_vertex_list;
		vector<vertex> grip_side_vertex_list;
		vector<vertex> grip_bot_vertex_list;

		for (int x = 0; x < m_segmentsX; x++) {
			// Top -- Note: builds in clock-wise direction

			v = guard_bot_vertex_list[x];
			// Bottom
			v.y = start_y - grip_length;
			grip_bot_vertex_list.push_back(v);

			//Side
			v = guard_bot_vertex_list[x];
			grip_side_vertex_list.push_back(v);
			// Wrap-around: sides collectively treated like a wrapped-up rectangle
			/* Order of filling:
			   Eg. for m_segmentsX = 3, m_segmentsY = 4:
				[[(0) 10 5 0]
				 [(1) 11 6 1]
				 [(2) 12 7 2]
				 [(3) 13 8 3]
				 [(4) 14 9 4]]
			*/
			for (int y = 1; y < m_segmentsY + 1; y++) {
				v.y = start_y - y * grip_length / m_segmentsY;
				grip_side_vertex_list.push_back(v);
			}
		}

		//v.y = start_y;
		//blade_bot_vertex_list.push_back(v);

//		vertex_list_list.push_back(grip_bot_vertex_list);
		vertex_list_list.push_back(grip_side_vertex_list);


		//Pommel
		float pommel_height = 1 - cone_size - blade_length - guard_height - grip_length;
		float pommel_width = 0.15;
		pommel_shift = start_y - grip_length - pommel_height / 2;
		start_x = 0;
//		start_y -= grip_length;
		start_z = 0;
		phiY = PI / m_segmentsY;
		vector<vertex> pommel_top_vertex_list = grip_bot_vertex_list;
		vector<vertex> pommel_bot_vertex_list;
		vector<vertex> pommel_side_vertex_list;
		layer_rads.clear();

		for (int y = 0; y < m_segmentsY + 1; y++) {
			layer_rads.push_back(sin(phiY * y) * pommel_width / 2);
		}

		for (int x = 0; x < m_segmentsX; x++) {
			// Bot -- Note: builds in clock-wise direction
			v.x = layer_rads[1] * cos(x * thetaX);
			v.z = layer_rads[1] * sin(x * thetaX);
			v.y = pommel_height / 2 * cos(phiY * (m_segmentsY - 1)) + pommel_shift;
			pommel_bot_vertex_list.push_back(v);

			//Side
//			v = pommel_bot_vertex_list[x];
			pommel_side_vertex_list.push_back(v);

			//Goes from 2nd layer to last layer (first layer done by Bot, last layer done for Top)
			for (int y = 2; y < m_segmentsY; y++) {
				v.y = pommel_height / 2 * cos(phiY * (m_segmentsY - y)) + pommel_shift;
				v.x = layer_rads[y] * cos(x * thetaX);
				v.z = layer_rads[y] * sin(x * thetaX);
				pommel_side_vertex_list.push_back(v);
			}
			//Add the top-most vertex to the guard_top_vertex_list
			pommel_top_vertex_list.push_back(v);
			// Wrap-around: sides collectively treated like a wrapped-up rectangle
			/* Order of filling:
			   Eg. for m_segmentsX = 3, m_segmentsY = 4:
				[[(4) 14 9 4]
				 [(3) 13 8 3]
				 [(2) 12 7 2]
				 [(1) 11 6 1]
				 [(0) 10 5 0]]
			*/
		}
		v.x = start_x;
		v.y = pommel_shift - pommel_height / 2;
		v.z = start_z;
		pommel_bot_vertex_list.push_back(v);
		pommel_top_vertex_list.push_back(pommel_top_vertex_list[m_segmentsX]);

		vertex_list_list.push_back(pommel_top_vertex_list);
		vertex_list_list.push_back(pommel_side_vertex_list);
		vertex_list_list.push_back(pommel_bot_vertex_list);


		//Triangles

		//Sword point
		vector<triangle> cone_side_tri_list;
		triangle t;
		for (int x = 0; x < m_segmentsX; x++) {
			// Bot
			//t.v_idx_list[0] = m_segmentsX;
			//t.v_idx_list[1] = x;
			//t.v_idx_list[2] = (x + 1) % m_segmentsX;
			//bot_tri_list.push_back(t);

			// Sides
			int num_side_vertices = m_segmentsX * (m_segmentsY + 1);
			for (int y = 0; y < m_segmentsY; y++) {
				// "Upper" triangle
				t.v_idx_list[0] = y + x * (m_segmentsY + 1);
				t.v_idx_list[1] = t.v_idx_list[0] + 1;
				t.v_idx_list[2] = (t.v_idx_list[0] + m_segmentsY + 2) % num_side_vertices;
				cone_side_tri_list.push_back(t);

				// "Lower" triangle
				t.v_idx_list[1] = t.v_idx_list[0];
				t.v_idx_list[0] = t.v_idx_list[2] - 1;
				cone_side_tri_list.push_back(t);
			}
		}
		triangle_list_list.push_back(cone_side_tri_list);


		//Blade
		vector<triangle> blade_side_tri_list;
		for (int x = 0; x < m_segmentsX; x++) {
						// Sides
			int num_side_vertices = m_segmentsX * (m_segmentsY + 1);
			for (int y = 0; y < m_segmentsY; y++) {
				// "Upper" triangle
				t.v_idx_list[0] = y + x * (m_segmentsY + 1);
				t.v_idx_list[1] = (t.v_idx_list[0] + m_segmentsY + 1) % num_side_vertices;
				t.v_idx_list[2] = t.v_idx_list[0] + 1;
				blade_side_tri_list.push_back(t);

				// "Lower" triangle
				t.v_idx_list[0] = t.v_idx_list[1] + 1;
				t.v_idx_list[2] = t.v_idx_list[1];
				t.v_idx_list[1] = y + x * (m_segmentsY + 1) + 1;
				blade_side_tri_list.push_back(t);
			}
		}
		triangle_list_list.push_back(blade_side_tri_list);


		//Guard
		vector<triangle> guard_top_tri_list;
		vector<triangle> guard_side_tri_list;
		for (int x = 0; x < m_segmentsX; x++) {
			//Top
			// "Lower" triangle
			t.v_idx_list[0] = x;
			t.v_idx_list[1] = (x + m_segmentsX + 1);// % (m_segmentsX * 2);
			t.v_idx_list[2] = x + m_segmentsX;
			guard_top_tri_list.push_back(t);
			// "Upper" triangle
			t.v_idx_list[0] = x;
			t.v_idx_list[2] = t.v_idx_list[1];
			t.v_idx_list[1] = (x + 1) % m_segmentsX;
			guard_top_tri_list.push_back(t);

			// Sides
			int num_side_vertices = m_segmentsX * (m_segmentsY - 1);
			int side_height = m_segmentsY - 1;
			for (int y = 0; y < side_height - 1; y++) {
				// "Upper" triangle
				t.v_idx_list[0] = y + x * (side_height);
				t.v_idx_list[1] = (t.v_idx_list[0] + side_height + 1) % num_side_vertices;
				t.v_idx_list[2] = t.v_idx_list[1] - 1;
				guard_side_tri_list.push_back(t);

				// "Lower" triangle
				t.v_idx_list[2] = t.v_idx_list[1];
				t.v_idx_list[1] = t.v_idx_list[0] + 1;
				guard_side_tri_list.push_back(t);
			}
		}
		triangle_list_list.push_back(guard_top_tri_list);
		triangle_list_list.push_back(guard_side_tri_list);


		//grip
		vector<triangle> grip_side_tri_list;
		for (int x = 0; x < m_segmentsX; x++) {
			// Sides
			int num_side_vertices = m_segmentsX * (m_segmentsY + 1);
			for (int y = 0; y < m_segmentsY; y++) {
				// "Upper" triangle
				t.v_idx_list[0] = y + x * (m_segmentsY + 1);
				t.v_idx_list[1] = (t.v_idx_list[0] + m_segmentsY + 1) % num_side_vertices;
				t.v_idx_list[2] = t.v_idx_list[0] + 1;
				grip_side_tri_list.push_back(t);

				// "Lower" triangle
				t.v_idx_list[0] = t.v_idx_list[1] + 1;
				t.v_idx_list[2] = t.v_idx_list[1];
				t.v_idx_list[1] = y + x * (m_segmentsY + 1) + 1;
				grip_side_tri_list.push_back(t);
			}
		}
		triangle_list_list.push_back(grip_side_tri_list);


		//Pommel
		vector<triangle> pommel_top_tri_list;
		vector<triangle> pommel_side_tri_list;
		vector<triangle> pommel_bot_tri_list;
		for (int x = 0; x < m_segmentsX; x++) {
			//Top
			// "Lower" triangle
			t.v_idx_list[0] = x;
			t.v_idx_list[1] = (x + m_segmentsX + 1);// % (m_segmentsX * 2);
			t.v_idx_list[2] = x + m_segmentsX; 
			pommel_top_tri_list.push_back(t);
			// "Upper" triangle
			t.v_idx_list[0] = x;
			t.v_idx_list[2] = t.v_idx_list[1];
			t.v_idx_list[1] = (x + 1) % m_segmentsX;
			pommel_top_tri_list.push_back(t);

			// Bot
			t.v_idx_list[0] = m_segmentsX;
			t.v_idx_list[1] = x;
			t.v_idx_list[2] = (x + 1) % m_segmentsX;
			pommel_bot_tri_list.push_back(t);

			// Sides
			int num_side_vertices = m_segmentsX * (m_segmentsY - 1);
			int side_height = m_segmentsY - 1;
			for (int y = 0; y < side_height - 1; y++) {
				// "Upper" triangle
				t.v_idx_list[0] = y + x * (side_height);
				t.v_idx_list[1] = (t.v_idx_list[0] + side_height + 1) % num_side_vertices;
				t.v_idx_list[2] = t.v_idx_list[1] - 1;
				pommel_side_tri_list.push_back(t);

				// "Lower" triangle
				t.v_idx_list[2] = t.v_idx_list[1];
				t.v_idx_list[1] = t.v_idx_list[0] + 1;
				pommel_side_tri_list.push_back(t);
			}
		}
		triangle_list_list.push_back(pommel_top_tri_list);
		triangle_list_list.push_back(pommel_side_tri_list);
		triangle_list_list.push_back(pommel_bot_tri_list);
	}

	void draw() {
		if (vertex_list_list.size() == 0 || triangle_list_list.size() == 0 || vertex_list_list[0].size() == 0 || triangle_list_list[0].size() == 0) {
			cerr << "Vertex list or triangle list(s) empty." << endl;
			return;
		}
		int v_count = 3; // triangle_list_list[0][0].v_count
		int num_areas = triangle_list_list.size(); //@TODO: should be 2
		int num_tris;

		for (int area = 0; area < num_areas; area++) {
			float xs[3];
			float ys[3];
			float zs[3];

			num_tris = triangle_list_list[area].size();
			for (int tri = 0; tri < num_tris; tri++) {
				for (int v_idx = 0; v_idx < v_count; v_idx++) {
					xs[v_idx] = vertex_list_list[area][triangle_list_list[area][tri].v_idx_list[v_idx]].x;
					ys[v_idx] = vertex_list_list[area][triangle_list_list[area][tri].v_idx_list[v_idx]].y;
					zs[v_idx] = vertex_list_list[area][triangle_list_list[area][tri].v_idx_list[v_idx]].z;
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
		if (vertex_list_list.size() == 0) {
			return;
		}
		float theta = 2 * PI / m_segmentsX;
		int num_faces = vertex_list_list.size();
		float normal[3] = { 0,0,0 };
		for (int face = 0; face < num_faces; face++) {
			int num_points = vertex_list_list[face].size();
			if (face == 0) {
				float px = 2;
				float py = 1;
				float pz = 0;

				//rotate normal
				for (int i = 0; i < num_points; i++) {
					if (i % (m_segmentsY + 1) == m_segmentsY)
						continue;
					int nth_side = i / (m_segmentsY + 1);
					vertex v = vertex_list_list[face][i];
					normal[0] = px * cos(-nth_side * theta) + pz * sin(-nth_side * theta);
					normal[1] = py;
					normal[2] = -px * sin(-nth_side * theta) + pz * cos(-nth_side * theta);
					normalizeNormal(normal[0], normal[1], normal[2]);
					normal[0] *= 0.01;
					normal[1] *= 0.01;
					normal[2] *= 0.01;
					glBegin(GL_LINES);
					glVertex3f(v.x, v.y, v.z); glVertex3f(normal[0] + v.x, normal[1] + v.y, normal[2] + v.z);
					glEnd();
				}
			}
			else if (face == 1 || face == 4) {
				for (int i = 0; i < num_points; i++) {
					vertex v = vertex_list_list[face][i];
					//find vector between v and center.
					normal[0] = v.x;
					normal[1] = 0;
					normal[2] = v.z;
					normalizeNormal(normal[0], normal[1], normal[2]);
					normal[0] *= 0.4;
					normal[2] *= 0.4;

					glBegin(GL_LINES);
					glVertex3f(v.x, v.y, v.z); glVertex3f(normal[0] + v.x, normal[1] + v.y, normal[2] + v.z);
					glEnd();
				}
			}
			else {
				for (int i = 0; i < num_points; i++) {
					vertex v = vertex_list_list[face][i];
					normal[0] = v.x;
					normal[2] = v.z;
					if (face == 3) {
						normal[1] = v.y - guard_shift;
					}
					else {
						normal[1] = v.y - pommel_shift;
					}
					normalizeNormal(normal[0], normal[1], normal[2]);
					normal[0] *= 0.1;
					normal[1] *= 0.1;
					normal[2] *= 0.1;
					glBegin(GL_LINES);
					glVertex3f(v.x, v.y, v.z); glVertex3f(normal[0] + v.x, normal[1] + v.y, normal[2] + v.z);
					glEnd();
				}
			}

		}
	};

private:
};

#endif