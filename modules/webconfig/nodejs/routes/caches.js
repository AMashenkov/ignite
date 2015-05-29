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

var router = require('express').Router();
var db = require('../db');

/**
 * Send spaces and caches accessed for user account.
 *
 * @param req Request.
 * @param res Response.
 */
function selectAll(req, res) {
    var user_id = req.user._id;

    // Get owned space and all accessed space.
    db.Space.find({$or: [{owner: user_id}, {usedBy: {$elemMatch: {account: user_id}}}]}, function (err, spaces) {
        if (err)
            return res.status(500).send(err);

        var space_ids = spaces.map(function(value, index) {
            return value._id;
        });

        // Get all caches for spaces.
        db.Cache.find({space: {$in: space_ids}}, function (err, caches) {
            if (err)
                return res.status(500).send(err);

            res.json({spaces: spaces, caches: caches});
        });
    });
}

/**
 * Get spaces and caches accessed for user account.
 */
router.get('/', function(req, res) {
    selectAll(req, res);
});

/**
 * Save cache.
 */
router.post('/save', function(req, res) {
    db.upsert(db.Cache, req.body, function(err) {
        if (err)
            return res.status(500).send(err);

        selectAll(req, res);
    });
});

/**
 * Remove cache by ._id.
 */
router.post('/remove', function(req, res) {
    db.Cache.remove(req.body, function (err) {
        if (err)
            return res.send(err);

        selectAll(req, res);
    })
});

module.exports = router;