import sys

def get_normal(v0, v1, v2):
    # get two vectors on the plane
    p0 = [v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]]
    p1 = [v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]]
    # cross product
    r = [0, 0, 0]
    r[0] = p0[1] * p1[2] - p0[2] * p1[1]
    r[1] = p0[2] * p1[0] - p0[0] * p1[2]
    r[2] = p0[0] * p1[1] - p0[1] * p1[0]
    return r

def create(infile, outfile):
    # read in data
    verts = []
    normals = []
    faces = []
    for line in infile:
        if line.startswith('v '):
            v = map(lambda x: x + 'f', filter(lambda x: len(x) > 0, line[2:].replace('\n', '').split(' ')))
            verts.append(v)
        elif line.startswith('vn '):
            vn = filter(lambda x: len(x) > 0, line[3:].replace('\n', '').split(' '))
            normals.append(vn)
        elif line.startswith('f '):
            f = map(lambda x: int(x) - 1, filter(lambda x: len(x) > 0, line[2:].replace('\n', '').split(' ')))
            faces.append(f)
            
    # spit verts back out
    print(len(verts))
    for f in faces:
        outfile.write('{0}, {1}, {2},\n'.format(', '.join(verts[f[0]]), ', '.join(verts[f[1]]), ', '.join(verts[f[2]])))
        
    # and then normals
    

if __name__ == '__main__':
    print(sys.argv[1])
    infile = open(sys.argv[1], 'r')
    outfile = open(sys.argv[1] + '.h', 'w')
    create(infile, outfile)
    infile.close()
    outfile.close()

    