project(
    'drop',
    'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++17',
        'warning_level=3',
    ],
)

commit = '(unknown)'
tag = '(unknown)'

commit_command = run_command(
    ['git', 'rev-parse', 'HEAD'],
    capture: true,
    check: false,
)

if commit_command.returncode() == 0
    commit = commit_command.stdout().strip()
endif

tag_command = run_command(
    ['git', 'describe', '--exact-match', '--tags'],
    capture: true,
    check: false,
)

if tag_command.returncode() == 0
    commit = tag_command.stdout().strip()
endif

version_header = configure_file(
    input: 'version.hpp.in',
    output: 'version.hpp',
    configuration: {
        'DROP_VERSION': meson.project_version(),
        'DROP_COMMIT_HASH': commit,
        'DROP_TAG': tag,
    },
)

sources = [
    'main.cpp',
]

executable('drop', sources, version_header, install: true)
