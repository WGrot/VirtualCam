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


add_cube_to_scene_file("Scene1.txt", center=(-1.5, 0.0, 5.0), length=1.0)
