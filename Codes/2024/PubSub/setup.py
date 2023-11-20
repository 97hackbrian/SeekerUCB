from setuptools import setup

package_name = 'PubSub'

setup(
    name=package_name,
    version='0.0.0',
    packages=[],
    py_modules=[
        'PubSub.publisher_node',
        'PubSub.subscriber_node',
    ],
    install_requires=[
        'setuptools',
        'rosdep',
        'rosdistro',
        'colcon-common-extensions',
    ],
    zip_safe=True,
    author='tu_nombre',
    author_email='tu_email@example.com',
    maintainer='tu_nombre',
    maintainer_email='tu_email@example.com',
    keywords=['ROS'],
    classifiers=[
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
    ],
    description='Un simple paquete ROS 2 con un nodo publicador y un nodo suscriptor',
    license='Apache License, Version 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'publisher_node = PubSub.publisher_node:main',
            'subscriber_node = PubSub.subscriber_node:main',
        ],
    },
)
