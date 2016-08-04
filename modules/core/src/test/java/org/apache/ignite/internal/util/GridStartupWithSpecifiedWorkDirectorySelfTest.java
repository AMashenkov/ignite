/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.ignite.internal.util;

import java.io.File;
import java.util.UUID;
import junit.framework.TestCase;
import org.apache.ignite.Ignite;
import org.apache.ignite.IgniteLogger;
import org.apache.ignite.IgniteSystemProperties;
import org.apache.ignite.configuration.IgniteConfiguration;
import org.apache.ignite.internal.util.typedef.G;
import org.apache.ignite.internal.util.typedef.X;
import org.apache.ignite.internal.util.typedef.internal.U;
import org.apache.ignite.logger.java.JavaLogger;
import org.apache.ignite.spi.discovery.tcp.TcpDiscoverySpi;
import org.apache.ignite.spi.discovery.tcp.ipfinder.TcpDiscoveryIpFinder;
import org.apache.ignite.spi.discovery.tcp.ipfinder.vm.TcpDiscoveryVmIpFinder;

import static org.apache.ignite.IgniteSystemProperties.IGNITE_HOME;
import static org.apache.ignite.internal.util.IgniteUtils.nullifyHomeDirectory;

/**
 * Checks creation of work folder.
 */
public class GridStartupWithSpecifiedWorkDirectorySelfTest extends TestCase {
    /** */
    private static final TcpDiscoveryIpFinder IP_FINDER = new TcpDiscoveryVmIpFinder(true);

    /** */
    private static final int GRID_COUNT = 2;

    /** System temp directory. */
    private static final String TMP_DIR = System.getProperty("java.io.tmpdir");

    /** {@inheritDoc} */
    @Override protected void setUp() throws Exception {
        // Protection against previously cached values.
        nullifyHomeDirectory();
    }

    /** {@inheritDoc} */
    @Override protected void tearDown() throws Exception {
        // Next grid in the same VM shouldn't use cached values produced by these tests.
        nullifyHomeDirectory();
    }

    /**
     * @param log Grid logger.
     * @return Grid configuration.
     */
    private IgniteConfiguration getConfiguration(IgniteLogger log) {
        // We can't use U.getIgniteHome() here because
        // it will initialize cached value which is forbidden to override.
        String ggHome = IgniteSystemProperties.getString(IGNITE_HOME);

        assert ggHome != null;

        U.setIgniteHome(null);

        String ggHome0 = U.getIgniteHome();

        assert ggHome0 == null;

        TcpDiscoverySpi disc = new TcpDiscoverySpi();

        disc.setIpFinder(IP_FINDER);

        IgniteConfiguration cfg = new IgniteConfiguration();

        cfg.setGridLogger(log);
        cfg.setDiscoverySpi(disc);

        return cfg;
    }

    /**
     * @throws Exception If failed.
     */
    public void testStartStopWithUndefinedHomeAndWorkDirs() throws Exception {
        IgniteLogger log = new JavaLogger();

        log.info(">>> Test started: " + getName());
        log.info("Grid start-stop test count: " + GRID_COUNT);

        File testWorkDir = null;

        try {
            for (int i = 0; i < GRID_COUNT; i++) {
                IgniteConfiguration cfg = getConfiguration(log);
                try (Ignite g = G.start(cfg)) {
                    assert g != null;

                    testWorkDir = U.resolveWorkDirectory(cfg.getWorkDirectory(), getName(), true);

                    assertTrue("Work directory wasn't created", testWorkDir.exists());

                    assertTrue("Work directory must be located in OS temp directory",
                        testWorkDir.getAbsolutePath().startsWith(TMP_DIR));

                    System.out.println(testWorkDir);

                    X.println("Stopping grid " + g.cluster().localNode().id());
                }
            }
        }
        finally {
            if (testWorkDir != null && testWorkDir.getAbsolutePath().startsWith(TMP_DIR))
                U.delete(testWorkDir);
        }
    }

    /**
     * @throws Exception If failed.
     */
    public void testStartStopWithUndefinedHomeAndConfiguredWorkDirs() throws Exception {
        IgniteLogger log = new JavaLogger();

        log.info(">>> Test started: " + getName());
        log.info("Grid start-stop test count: " + GRID_COUNT);

        String tmpWorkDir = new File(TMP_DIR, getName() + "_" + UUID.randomUUID()).getAbsolutePath();

        try {
            for (int i = 0; i < GRID_COUNT; i++) {
                IgniteConfiguration cfg = getConfiguration(log);

                cfg.setWorkDirectory(tmpWorkDir);

                try (Ignite g = G.start(cfg)) {
                    assert g != null;

                    File testWorkDir = U.resolveWorkDirectory(cfg.getWorkDirectory(), getName(), true);

                    assertTrue("Work directory wasn't created", testWorkDir.exists());

                    assertTrue("Work directory must be located in configured directory",
                        testWorkDir.getAbsolutePath().startsWith(tmpWorkDir));

                    X.println("Stopping grid " + g.cluster().localNode().id());
                }
            }
        } finally {
            U.delete(new File(tmpWorkDir));
        }
    }
}