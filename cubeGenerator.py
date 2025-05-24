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


def add_sphere_faces_to_scene_file(filename, center, radius, lat_segments=8, lon_segments=8):
    cx, cy, cz = center
    vertices = []


    for i in range(lat_segments + 1):
        theta = math.pi * i / lat_segments  
        for j in range(lon_segments):
            phi = 2 * math.pi * j / lon_segments  
            x = cx + radius * math.sin(theta) * math.cos(phi)
            y = cy + radius * math.sin(theta) * math.sin(phi)
            z = cz + radius * math.cos(theta)
            vertices.append((x, y, z))

    def get_index(i, j):
        return i * lon_segments + (j % lon_segments)

    with open(filename, 'a') as file:
        file.write('\n# New sphere (random colored triangles)\n')
        for i in range(lat_segments):
            for j in range(lon_segments):
                R = 128#random.randint(0, 255)
                G = 128#random.randint(0, 255)
                B = 128#random.randint(0, 255)

                v1 = vertices[get_index(i, j)]
                v2 = vertices[get_index(i + 1, j)]
                v3 = vertices[get_index(i + 1, j + 1)]
                v4 = vertices[get_index(i, j + 1)]


                file.write(f"{v1[0]} {v1[1]} {v1[2]} {v2[0]} {v2[1]} {v2[2]} {v3[0]} {v3[1]} {v3[2]} {R} {G} {B}\n")

                file.write(f"{v1[0]} {v1[1]} {v1[2]} {v3[0]} {v3[1]} {v3[2]} {v4[0]} {v4[1]} {v4[2]} {R} {G} {B}\n")


add_sphere_faces_to_scene_file("phong.txt", center=(0, 0.0, 20.0), radius=2.0)
#add_cube_faces_to_scene_file("scene.txt", center=(0, 0.0, 20.0), length=4.0)
#add_cube_to_scene_file("scene.txt", center=(-1.5, 0.0, 5.0), length=1.0)