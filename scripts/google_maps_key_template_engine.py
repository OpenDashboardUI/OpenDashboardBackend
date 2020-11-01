#! /usr/bin/python3

from string import Template
from sys import exit
from pathlib import Path
import argparse

QRC_TEMPLATE_STRING = """
<RCC>
    <qresource prefix = "/maps">
        <file>${maps_filename}</file>
    </qresource>
</RCC>
"""


def generate_maps_html_file(input_file_path: str, output_file_path: str, api_key: str):
    input_file = open(input_file_path, "r")
    output_file = open(output_file_path, "w")

    input_string = input_file.read()

    html_template = Template(input_string)
    output_string = html_template.substitute(api_key=api_key)

    output_file.write(output_string)


def generate_qrc_file(output_file_path: str):

    output_file_dir = Path(output_file_path).parent
    output_file_name = Path(output_file_path).name
    qrc_file_path = output_file_dir / "map.qrc"

    qrc_file = open(qrc_file_path, "w")
    qrc_template = Template(QRC_TEMPLATE_STRING)
    qrc_output_string = qrc_template.substitute(maps_filename=output_file_name)
    qrc_file.write(qrc_output_string)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Render HTML file with Google Maps API Key from template.')
    parser.add_argument('--input', dest='input', help='Input template file.')
    parser.add_argument('--output', dest='output', help='Output HTML file.')
    parser.add_argument('--key', dest='key', default=None, nargs='?', help='Google Maps API key.')
    args = parser.parse_args()

    if args.input is None or args.output is None:
        print("Missing input parameter 'input' or 'output'.")
        exit(-1)

    generate_maps_html_file(args.input, args.output, args.key)
    generate_qrc_file(args.output)
