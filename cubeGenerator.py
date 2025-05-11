import random

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
add_cube_faces_to_scene_file("scene.txt", center=(-3, 0.0, 7.0), length=1.0)
#add_cube_to_scene_file("scene.txt", center=(-1.5, 0.0, 5.0), length=1.0)