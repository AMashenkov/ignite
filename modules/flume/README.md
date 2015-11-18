#Flume NG sink

## Setting up and running

1. Create a transformer by implementing EventTransformer interface.
2. Build it and copy to ${FLUME_HOME}/plugins.d/ignite-sink/lib.
3. Copy other Ignite-related jar files to ${FLUME_HOME}/plugins.d/ignite-sink/libext to have them as shown below.

```
plugins.d/
`-- ignite
    |-- lib
    |   `-- ignite-flume-transformer-x.x.x.jar <-- your jar
    `-- libext
        |-- cache-api-1.0.0.jar
        |-- ignite-core-x.x.x.jar
        |-- ignite-flume-x.x.x.jar
        |-- ignite-spring-x.x.x.jar
        |-- spring-aop-4.1.0.RELEASE.jar
        |-- spring-beans-4.1.0.RELEASE.jar
        |-- spring-context-4.1.0.RELEASE.jar
        |-- spring-core-4.1.0.RELEASE.jar
        `-- spring-expression-4.1.0.RELEASE.jar
```

4. In Flume configuration file, specify Ignite configuration XML file's location with cache properties
(see [Apache Ignite](https://apacheignite.readme.io/) with cache name specified for cache creation,
cache name (same as in Ignite configuration file), your EventTransformer's implementation class,
and, optionally, batch size (default -- 100).

```
# Describe the sink
a1.sinks.k1.type = org.apache.ignite.stream.flume.IgniteSink
a1.sinks.k1.igniteCfg = /some-path/ignite.xml
a1.sinks.k1.cacheName = testCache
a1.sinks.k1.eventTransformer = my.company.MyEventTransformer
a1.sinks.k1.batchSize = 100
```

After specifying your source and channel (see Flume's docs), you are ready to run a Flume agent.