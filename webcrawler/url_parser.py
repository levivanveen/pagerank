infile = open('crawlerout.txt', 'r')

# Get url tokens
lines = infile.readlines()
token_list = []
for line in lines:
    if line != []:
        line_tokens = line.split()
        token_list.append(line_tokens)

url_ref = {}

# Insert url tokens to dictionary (url: link1, link2, etc)
for row in token_list:
    for token in row:
        if token not in url_ref:
            url_ref[token] = ""

for item_list in token_list:
    parent_url = item_list[0]
    del item_list[0]
    url_ref[parent_url] = item_list

# make reference list of keys
keys_outfile = open("crawler_keys.txt", "w")
key_list = []
for key, values in url_ref.items():
    keys_outfile.write(f"{key}\n")
    key_list.append(key)
keys_outfile.close()

# count links 
link_array = [[0 for col in range(len(url_ref))] for row in range(len(url_ref))]
row_count = 0
for key, values in url_ref.items():
    for val in values:
        link_array[row_count][key_list.index(val)] += 1
    row_count += 1


grid_outfile = open("grid.txt", "w")
for i in range(len(link_array)):
    for j in range(len(link_array)):
        grid_outfile.write(f"{link_array[i][j]} ")
    grid_outfile.write("\n")
