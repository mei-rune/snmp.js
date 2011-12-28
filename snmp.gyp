{
  'variables': {
    'v8_use_snapshot': 'true',
    # Turn off -Werror in V8
    # See http://codereview.chromium.org/8159015
    'werror': '',
    'target_arch': 'ia32',
    'node_use_dtrace': 'false',
    'node_use_openssl%': 'true',
    'node_use_system_openssl%': 'false',
  
  },

  'targets': [
    {
      'target_name': 'snmp',
      'type': '<(library)',
      'include_dirs': [
        'src',
        'deps/net-snmp/include/',
        'deps/net-snmp/win32/',
        'deps/node/src/',
        'deps/node/deps/uv/include',
        'deps/node/deps/v8/include',
      ],

      'sources': [
        'src/binding.cpp',
        'src/session.cpp',
        'src/session.h',
        'src/pdu.cpp',
        'src/pdu.h',
        'src/transport.cpp',
        'src/stream_adapter.cpp',
        'src/stream_adapter.h',
        'src/snmp.h',
        'lib/index.js',
        'index.js',
        'package.json',
        'common.gypi',
      ],

      'defines': [
        'HAVE_CONFIG_H',
        'ARCH="<(target_arch)"',
        'PLATFORM="<(OS)"',
        '_LARGEFILE_SOURCE',
        '_FILE_OFFSET_BITS=64',
      ], 
	  'msbuild_settings': {
			'PostBuildEvent': {
				'Command': 'copy  /Y $(ProjectDir)$(Configuration)\\snmp.dll $(ProjectDir)lib\\snmp.node && copy /Y "$(ProjectDir)lib\\*.*" "$(ProjectDir)test\\node_modules\\snmp\\lib"',
			}
	  },
      'postbuilds':  {
            'postbuild_name': 'copy snmp to node_modules',
            'action': [
              'cp',
              '$(BUILT_PRODUCTS_DIR)/$(Configuration)/snmp.dll',
              '$(BUILT_PRODUCTS_DIR)/test\node_modules/snmp.node',
            ],
       },
	  

      'conditions': [
        [ 'OS=="win"', {
            'sources': [
              # headers to make for a more pleasant IDE experience
            ],
            'defines': [
              '_GNU_SOURCE',
              'HAVE_STDINT_H',
              'HAVE_SSIZE_T',
              'FD_SETSIZE=1024',
              # we need to use node's preferred "win32" rather than gyp's preferred "win"
              'PLATFORM="win32"',
              'BUILDING_NODE_EXTENSION'
            ],
            'libraries': [
              'Winmm.lib',
              'psapi.lib',
              'ws2_32.lib',
              'deps/node/$(Configuration)/node.lib',
              'deps/net-snmp/win32/lib/$(Configuration)/netsnmp.lib',
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
          }
        ],
      ],
    },
	
    {
      'target_name': 'run-tests',
      'type': 'none',
      'dependencies': [ 'snmp' ],
      'toolsets': ['host'],
      'variables': { },

      'sources': [
             'test/require.test.js',
             'test/pdu.test.js',
             'test/session.test.js',
      ],
	  
      'actions': [
        {
          'action_name': 'expresso',

          'inputs': [
             'test/require.test.js',
             'test/pdu.test.js',
             'test/session.test.js',
          ],

		  
          'outputs': [
            '$(OutDir)/test.result',
          ],

          'action': [
            '$(ProjectDir)deps/node/$(Configuration)/node.exe',
            '$(ProjectDir)tools/node_modules/expresso/bin/expresso',
            '$(FullPath)',
          ],
        },
	    ] # actions
    } # run-tests
  ] # targets
}


