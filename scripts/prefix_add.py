import os

# Prompt the user for the folder path
folder_path = raw_input("Enter the folder path: ")

# Prompt the user for the prefix
prefix = raw_input("Enter the prefix to add to the file names: ")

# List all files in the folder
files = os.listdir(folder_path)

# Iterate through the files and rename them with the prefix
for filename in files:
    if not filename.startswith(prefix):
        new_filename = prefix + filename
        old_filepath = os.path.join(folder_path, filename)
        new_filepath = os.path.join(folder_path, new_filename)
        os.rename(old_filepath, new_filepath)
        print 'Renamed: {} to {}'.format(filename, new_filename)
