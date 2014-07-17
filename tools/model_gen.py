import sys

def create(infile, outfile, name_prefix):
    # read in data
    verts = []
    normals = []
    faces = []
    for line in infile:
        if line.startswith('v '):
            v = map(lambda x: x + 'f', filter(lambda x: len(x) > 0, line[2:].replace('\n', '').split(' ')))
            verts.append(v)
        elif line.startswith('vn '):
            vn = map(lambda x: x + 'f', filter(lambda x: len(x) > 0, line[3:].replace('\n', '').split(' ')))
            normals.append(vn)
        elif line.startswith('f '):
            f = line[2:].replace('\n', '').split(' ')
            faces.append(f)
            
    num_elements = len(faces) * 9
            
    # spit verts back out
    outfile.write('// Verts\n')
    outfile.write('float {0}_verts[{1}] = {{\n'.format(name_prefix, num_elements))
    print(len(verts))
    for f in faces:
        index = map(lambda x: int(x.split('/')[0]) - 1, f)
        outfile.write('    {0}, {1}, {2},\n'.format(', '.join(verts[index[0]]), ', '.join(verts[index[1]]), ', '.join(verts[index[2]])))
    outfile.write('};\n\n')
        
    # and then normals
    outfile.write('// Normals\n')
    outfile.write('float {0}_normals[{1}] = {{\n'.format(name_prefix, num_elements))
    for f in faces:
        index = map(lambda x: int(x.split('/')[2]) - 1, f)
        outfile.write('    {0}, {1}, {2},\n'.format(', '.join(normals[index[0]]), ', '.join(normals[index[1]]), ', '.join(normals[index[2]])))
    outfile.write('};\n\n')
    

if __name__ == '__main__':
    print(sys.argv[1])
    infile = open(sys.argv[1], 'r')
    outfile = open(sys.argv[1] + '.h', 'w')
    create(infile, outfile, sys.argv[2])
    infile.close()
    outfile.close()

    