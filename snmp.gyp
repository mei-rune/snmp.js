{
  'target_defaults': {
    'conditions': [
      ['OS != "win"', {
        'defines': [
          '_LARGEFILE_SOURCE',
          '_FILE_OFFSET_BITS=64',
          '_GNU_SOURCE',
        ],
        'conditions': [
          ['OS=="solaris"', {
            'cflags': ['-pthreads'],
            'ldlags': ['-pthreads'],
          }, {
            'cflags': ['-pthread'],
            'ldlags': ['-pthread'],
          }],
        ],
      }],
    ],
  },

  'targets': [
    {
      'target_name': 'snmp',
      'type': '<(library)',
      'include_dirs': [
        'src/',
        'deps/net-snmp/include/',
        'deps/net-snmp/win32/',
        'deps/node/src/',
        'deps/node/deps/uv/include',
        'deps/node/deps/v8/include',
      ],
      'direct_dependent_settings': {
        'include_dirs': [ 'include' ],
      },

      'defines': [
        'HAVE_CONFIG_H',
        'ARCH="<(target_arch)"',
        'PLATFORM="<(OS)"',
        '_LARGEFILE_SOURCE',
        '_FILE_OFFSET_BITS=64',
      ],
      'sources': [
        'src/binding.cpp',
        'src/session.cpp',
        'src/snmp.h',
      ],
      'conditions': [
        [ 'OS=="win"', {
          'include_dirs': [
           'src/win/'
          ],
          'defines': [
            '_WIN32_WINNT=0x0502',
            '_GNU_SOURCE',
			'HAVE_STDINT_H',
			'HAVE_SSIZE_T',
            'FD_SETSIZE=1024',
            # we need to use node's preferred "win32" rather than gyp's preferred "win"
            'PLATFORM="win32"',
          ],
          'sources': [
           'src/platform_win32.h'
          ],
            'libraries': [
              'Winmm.lib',
              'psapi.lib',
              'ws2_32.lib',
              'deps/node/debug/lib/v8_base.lib',
              'deps/node/debug/lib/v8_snapshot.lib',
              'deps/net-snmp/win32/lib/debug/netsnmp.lib',
            ],
        }, { # Not Windows i.e. POSIX
          'cflags': [
            '-g',
            '--std=gnu89',
            '-pedantic',
            '-Wall',
            '-Wextra',
            '-Wno-unused-parameter'
          ],
          'sources': [
          ],
          'include_dirs': [ 
          ],
          'libraries': [ '-lm' ]
        }],
      ],
    },

    {
      'target_name': 'run-tests',
      'type': 'executable',
      'dependencies': [ 'snmp' ],
      'sources': [
      ],
      'conditions': [
        [ 'OS=="win"', {
          'sources': [
          ],
          'libraries': [ 'ws2_32.lib' ]
        }, { # POSIX
          'defines': [ '_GNU_SOURCE' ],
          'sources': [
          ]
        }]
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1, # /subsystem:console
        },
      },
    }
  ]
}


