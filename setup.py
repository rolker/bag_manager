import setuptools 

setuptools.setup( 
    name='msgext', 
    version='1.0', 
    author='Avery Munoz', 
    author_email='avery.munoz@unh.edu', 
    description='Prints msg info from bag files', 
    packages=setuptools.find_packages(), 
    entry_points={ 
        'console_scripts': [ 
            'msgext = msgext.msgext:main' 
        ] 
    }, 
    classifiers=[ 
        'Programming Language :: Python :: 3',  
        'Operating System :: OS Independent', 
    ], 
)
