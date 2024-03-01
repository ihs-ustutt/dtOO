# Configuration file for the Sphinx documentation builder.
import os
import sys

project = 'dtOO'
copyright = 'Alexander Tismer'
author = 'Alexander Tismer'
#release = '0.1'


extensions = [
  'myst_parser', 
  'breathe',
]

# Configuration for the breathe extension
# Which directory to read the Doxygen output from
breathe_projects = {'dtOO':'./xml'}
breathe_default_project = 'dtOO'

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_theme_options = {
  'logo_only': True,
}

html_logo = "pics/dtoo-logo-inv-nbg.png"
