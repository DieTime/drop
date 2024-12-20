configuration = configuration_data(
    {
        'DROP_VERSION': meson.project_version(),
        'DROP_COMMIT_HASH': '(unknown)',
        'DROP_TAG': '(unknown)',
        'DROP_REMOTE': '(unknown)',
    }
)

commit_command = run_command(
    ['git', 'rev-parse', 'HEAD'],
    capture: true,
    check: false,
)

if commit_command.returncode() == 0
    configuration.set('DROP_COMMIT_HASH', commit_command.stdout().strip())
endif

tag_command = run_command(
    ['git', 'describe', '--exact-match', '--tags'],
    capture: true,
    check: false,
)

if tag_command.returncode() == 0
    configuration.set('DROP_TAG', tag_command.stdout().strip())
endif

remote_command = run_command(
    ['git', 'config', '--get', 'remote.origin.url'],
    capture: true,
    check: false,
)

if remote_command.returncode() == 0
    configuration.set('DROP_REMOTE', remote_command.stdout().strip())
endif

version_header = configure_file(
    input: 'version.in.hpp',
    output: 'version.hpp',
    configuration: configuration,
)

sources = ['fs.cpp', 'print.cpp', 'trash_info.cpp', 'uuid.cpp', 'main.cpp']

executable(
    'drop',
    sources,
    version_header,
    install: true,
)
