#include "ImportedModel.h"

using namespace std;
// ------------  Imported Model class

ImportedModel::ImportedModel(const char* filePath) {
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath);
	// uses modelImporter to get vertex information
	numVertices = modelImporter.getNumVertices();
	std::vector<float> verts = modelImporter.getVertices();
	std::vector<float> tcs = modelImporter.getTextureCoordinates();
	std::vector<float> normals = modelImporter.getNormals();
	for (int i = 0; i < numVertices; i++) {
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
}
int ImportedModel::getNumVertices() { return numVertices; }
// accessors
std::vector<glm::vec3> ImportedModel::getVertices() { return vertices; }
std::vector<glm::vec2> ImportedModel::getTextureCoords() { return texCoords; }
std::vector<glm::vec3> ImportedModel::getNormals() { return normalVecs; }


// --------------  Model Importer class
ModelImporter::ModelImporter() {}

void ModelImporter::parseOBJ(const char* filePath) {
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0) {
			stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z;
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
			// vertex position ("v" case)
			// extract the vertex position values
		}
		if (line.compare(0, 2, "vt") == 0) { // texture coordinates ("vt" case)
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; // extract texture coordinate values
			stVals.push_back(x);
			stVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0) { // vertex normals ("vn" case)
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z; // extract the normal vector values
			normVals.push_back(x);
			normVals.push_back(y);
			normVals.push_back(z);
		}
		if (line.compare(0, 2, "f ") == 0) { // triangle faces ("f" case)
            string token;
            stringstream ss(line.erase(0, 2));

            // Parse exactly 3 corners (triangles). If your OBJ has quads, triangulate upstream.
            int vi[3] = { -1, -1, -1 };
            int ti[3] = { -1, -1, -1 };
            int ni[3] = { -1, -1, -1 };

            auto parseCorner = [](const string& s, int& v, int& t, int& n) {
                v = t = n = -1;
                // formats: v / v/t / v//n / v/t/n
                size_t p1 = s.find('/');
                if (p1 == string::npos) { // "v"
                    v = stoi(s);
                    return;
                }
                // we have at least one slash
                string a = s.substr(0, p1);
                v = a.empty() ? -1 : stoi(a);

                size_t p2 = s.find('/', p1 + 1);
                if (p2 == string::npos) { // "v/t"
                    string b = s.substr(p1 + 1);
                    t = b.empty() ? -1 : stoi(b);
                    return;
                }

                // "v//n" or "v/t/n"
                string b = s.substr(p1 + 1, p2 - (p1 + 1)); // may be empty
                string c = s.substr(p2 + 1);                // may be empty

                if (!b.empty()) t = stoi(b); // if empty -> v//n
                if (!c.empty()) n = stoi(c);
                };

            for (int i = 0; i < 3; ++i) {
                if (!(ss >> token)) break; // defensive
                parseCorner(token, vi[i], ti[i], ni[i]);
            }

            // Helper lambdas to safely fetch data (OBJ indices are 1-based; this ignores negative indices)
            auto V = [&](int idx, int comp) -> float {
                int base = (idx - 1) * 3 + comp;
                if (idx > 0 && base + (0) >= 0 && base < (int)vertVals.size()) return vertVals[base];
                return 0.0f;
                };
            auto VT = [&](int idx, int comp) -> float {
                int base = (idx - 1) * 2 + comp;
                if (idx > 0 && base >= 0 && base < (int)stVals.size()) return stVals[base];
                return 0.0f;
                };
            auto VN = [&](int idx, int comp) -> float {
                int base = (idx - 1) * 3 + comp;
                if (idx > 0 && base >= 0 && base < (int)normVals.size()) return normVals[base];
                return 0.0f;
                };

            // Gather triangle positions first (needed if we must compute a flat normal)
            float px[3][3];
            for (int i = 0; i < 3; ++i) {
                px[i][0] = V(vi[i], 0);
                px[i][1] = V(vi[i], 1);
                px[i][2] = V(vi[i], 2);
            }

            // If any corner lacks a normal, compute a flat face normal
            bool needFaceNormal = false;
            for (int i = 0; i < 3; ++i) {
                if (ni[i] <= 0) { needFaceNormal = true; break; }
            }
            float fn[3] = { 0.f, 0.f, 0.f };
            if (needFaceNormal) {
                // e1 = p2 - p1, e2 = p3 - p1
                float e1x = px[1][0] - px[0][0];
                float e1y = px[1][1] - px[0][1];
                float e1z = px[1][2] - px[0][2];
                float e2x = px[2][0] - px[0][0];
                float e2y = px[2][1] - px[0][1];
                float e2z = px[2][2] - px[0][2];
                // cross(e1, e2)
                fn[0] = e1y * e2z - e1z * e2y;
                fn[1] = e1z * e2x - e1x * e2z;
                fn[2] = e1x * e2y - e1y * e2x;
                // normalize
                float len = std::sqrt(fn[0] * fn[0] + fn[1] * fn[1] + fn[2] * fn[2]);
                if (len > 1e-12f) { fn[0] /= len; fn[1] /= len; fn[2] /= len; }
            }

            // Emit final buffers
            for (int i = 0; i < 3; ++i) {
                // Positions
                triangleVerts.push_back(px[i][0]);
                triangleVerts.push_back(px[i][1]);
                triangleVerts.push_back(px[i][2]);

                // Texcoords (0,0 if missing)
                float u = VT(ti[i], 0);
                float v = VT(ti[i], 1);
                textureCoords.push_back(u);
                textureCoords.push_back(v);

                // Normals: from file if present, else use face normal (if computed), else (0,0,1)
                if (ni[i] > 0) {
                    normals.push_back(VN(ni[i], 0));
                    normals.push_back(VN(ni[i], 1));
                    normals.push_back(VN(ni[i], 2));
                }
                else if (needFaceNormal) {
                    normals.push_back(fn[0]);
                    normals.push_back(fn[1]);
                    normals.push_back(fn[2]);
                }
                else {
                    normals.push_back(0.f);
                    normals.push_back(0.f);
                    normals.push_back(1.f);
                }
            }
		}
	}
}

int ModelImporter::getNumVertices() { return (triangleVerts.size() / 3); }  // accessors
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTextureCoordinates() { return textureCoords; }
std::vector<float> ModelImporter::getNormals() { return normals; }
