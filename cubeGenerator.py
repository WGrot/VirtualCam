import random
import math

def add_cube_to_scene_file(filename, center, length):
    cx, cy, cz = center
    l = length / 2.0

    vertices = [
        (cx - l, cy - l, cz - l),
        (cx + l, cy - l, cz - l),
        (cx + l, cy + l, cz - l),
        (cx - l, cy + l, cz - l),
        (cx - l, cy - l, cz + l),
        (cx + l, cy - l, cz + l),
        (cx + l, cy + l, cz + l),
        (cx - l, cy + l, cz + l),
    ]
    
    edges = [
        (0, 1), (1, 2), (2, 3), (3, 0),  
        (4, 5), (5, 6), (6, 7), (7, 4),  
        (0, 4), (1, 5), (2, 6), (3, 7)   
    ]

    with open(filename, 'a') as file:
        file.write('\n# New cube\n')
        for start_idx, end_idx in edges:
            x1, y1, z1 = vertices[start_idx]
            x2, y2, z2 = vertices[end_idx]
            line = f"{x1} {y1} {z1} {x2} {y2} {z2}\n"
            file.write(line)


def add_cube_faces_to_scene_file(filename, center, length):
    cx, cy, cz = center
    l = length / 2.0

    
    vertices = [
        (cx - l, cy - l, cz - l),  
        (cx + l, cy - l, cz - l),  
        (cx + l, cy + l, cz - l),  
        (cx - l, cy + l, cz - l),  
        (cx - l, cy - l, cz + l),  
        (cx + l, cy - l, cz + l),  
        (cx + l, cy + l, cz + l),  
        (cx - l, cy + l, cz + l),  
    ]

    
    triangles_by_face = [
        [(0, 2, 1), (0, 3, 2)],  
        [(4, 5, 6), (4, 6, 7)],  
        [(0, 1, 5), (0, 5, 4)],  
        [(2, 3, 7), (2, 7, 6)],  
        [(0, 4, 7), (0, 7, 3)],  
        [(1, 2, 6), (1, 6, 5)],  
    ]

    with open(filename, 'a') as file:
        file.write('\n# New cube (random colored triangles)\n')
        for face in triangles_by_face:
            R = random.randint(0, 255)
            G = random.randint(0, 255)
            B = random.randint(0, 255)
            for v1_idx, v2_idx, v3_idx in face:
                v1 = vertices[v1_idx]
                v2 = vertices[v2_idx]
                v3 = vertices[v3_idx]
                line = f"{v1[0]} {v1[1]} {v1[2]} {v2[0]} {v2[1]} {v2[2]} {v3[0]} {v3[1]} {v3[2]} {R} {G} {B}\n"
                file.write(line)


def normalize(v):
    length = math.sqrt(sum(coord ** 2 for coord in v))
    return tuple(coord / length for coord in v)

def midpoint(v1, v2):
    return normalize(tuple((a + b) / 2.0 for a, b in zip(v1, v2)))

def create_icosphere(center, radius, subdivisions):
    t = (1.0 + math.sqrt(5.0)) / 2.0
    raw_vertices = [
        (-1,  t,  0), (1,  t,  0), (-1, -t,  0), (1, -t,  0),
        (0, -1,  t), (0, 1,  t), (0, -1, -t), (0, 1, -t),
        ( t,  0, -1), ( t,  0, 1), (-t,  0, -1), (-t,  0, 1),
    ]
    vertices = [normalize(v) for v in raw_vertices]

    faces = [
        (0, 11, 5), (0, 5, 1), (0, 1, 7), (0, 7, 10), (0, 10, 11),
        (1, 5, 9), (5, 11, 4), (11, 10, 2), (10, 7, 6), (7, 1, 8),
        (3, 9, 4), (3, 4, 2), (3, 2, 6), (3, 6, 8), (3, 8, 9),
        (4, 9, 5), (2, 4, 11), (6, 2, 10), (8, 6, 7), (9, 8, 1)
    ]

    middle_point_cache = {}

    def get_middle_point(i1, i2):
        key = tuple(sorted((i1, i2)))
        if key in middle_point_cache:
            return middle_point_cache[key]
        v1 = vertices[i1]
        v2 = vertices[i2]
        mid = midpoint(v1, v2)
        vertices.append(mid)
        idx = len(vertices) - 1
        middle_point_cache[key] = idx
        return idx

    for _ in range(subdivisions):
        new_faces = []
        for tri in faces:
            a = get_middle_point(tri[0], tri[1])
            b = get_middle_point(tri[1], tri[2])
            c = get_middle_point(tri[2], tri[0])
            new_faces.extend([
                (tri[0], a, c),
                (tri[1], b, a),
                (tri[2], c, b),
                (a, b, c)
            ])
        faces = new_faces

    final_vertices = [(
        center[0] + v[0] * radius,
        center[1] + v[1] * radius,
        center[2] + v[2] * radius
    ) for v in vertices]

    return final_vertices, faces

def save_icosphere_to_file(filename, center, radius, subdivisions):
    vertices, faces = create_icosphere(center, radius, subdivisions)

    with open(filename, 'a') as file:
        file.write("\n# New icosphere\n")
        for i1, i2, i3 in faces:
            v1 = vertices[i1]
            v2 = vertices[i2]
            v3 = vertices[i3]
            R = 128#random.randint(0, 255)
            G = 128#random.randint(0, 255)
            B = 128#random.randint(0, 255)
            line = f"{v1[0]} {v1[1]} {v1[2]} {v2[0]} {v2[1]} {v2[2]} {v3[0]} {v3[1]} {v3[2]} {R} {G} {B}\n"
            file.write(line)

save_icosphere_to_file("phong.txt", center=(0, 0, 5), radius=1.0, subdivisions=1)


#add_sphere_faces_to_scene_file("phong.txt", center=(0, 0.0, 20.0), radius=4.0)
#add_cube_faces_to_scene_file("scene.txt", center=(0, 0.0, 20.0), length=4.0)
#add_cube_to_scene_file("scene.txt", center=(-1.5, 0.0, 5.0), length=1.0)