mystr = ''

# Read
with open('expr.out', 'r') as f:
  lines = f.readlines()
  mystr = ''.join([line.strip() for line in lines]).replace(' ','')
  mystr = mystr.replace(';',';\n')

# Write
with open('expr.txt', 'w') as f:
  f.write(mystr)

