import argparse
import os
import json


def collect_translation_strings(filename):
    strings = []

    with open(filename, 'r') as file:
        project = json.load(file)
        strings.append(project["name"])

        for category in project["categories"]:
            strings.append(category["name"])

            for unit in category["units"]:
                strings.append(unit["name"])

    return strings


def write_strings_to_file(filename, strings):
    with open(filename, 'w') as file:
        for string in strings:
            file.write('qsTranslate("Units", "{}")\n'.format(string))


parser = argparse.ArgumentParser(
    prog='Unit Translation Builder',
    description='Parses a unit .json file and extracts all the strings into a .js file so that they can be translated')
parser.add_argument('filename')
args = parser.parse_args()

translation_strings = collect_translation_strings(args.filename)
print(translation_strings)

translation_file = os.path.splitext(args.filename)[0]+'_ts.js'
print("\nWriting translation strings to: " + translation_file)

write_strings_to_file(translation_file, translation_strings)
