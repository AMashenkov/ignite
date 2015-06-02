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

var config = require('./configuration.js');

// Mongoose for mongodb.
var mongoose = require('mongoose'),
    Schema = mongoose.Schema,
    ObjectId = mongoose.Schema.Types.ObjectId,
    passportLocalMongoose = require('passport-local-mongoose');

// Connect to mongoDB database.
mongoose.connect(config.get('mongoDB:url'), {server: {poolSize: 4}});

// Define account model.
var AccountSchema = new Schema({
    username: String
});

AccountSchema.plugin(passportLocalMongoose, { usernameField: 'email' });

exports.Account = mongoose.model('Account', AccountSchema);

// Define space model.
exports.Space =  mongoose.model('Space', new Schema({
    name: String,
    owner: { type: ObjectId, ref: 'Account' },
    usedBy: [{
        permission: { type: String, enum: ['VIEW', 'FULL']},
        account: { type: ObjectId, ref: 'Account' }
    }]
}));

var DiscoveryObj = {
    className: String, enum: ['TcpDiscoveryVmIpFinder', 'TcpDiscoveryMulticastIpFinder', 'TcpDiscoveryS3IpFinder',
        'TcpDiscoveryCloudIpFinder', 'TcpDiscoveryGoogleStorageIpFinder', 'TcpDiscoveryJdbcIpFinder',
        'TcpDiscoverySharedFsIpFinder'],
    addresses: [String]
};

// Define discovery model.
exports.Discovery =  mongoose.model('Discovery', new Schema(DiscoveryObj));

var ClusterSchema = new Schema({
    space: { type: ObjectId, ref: 'Space' },
    name: String,
    discovery: {
        kind: { type: String, enum: ['Vm', 'Multicast', 'S3', 'Cloud', 'GoogleStorage', 'Jdbc', 'SharedFs'] },
        addresses: [String]
    },
    pubPoolSize: Number,
    sysPoolSize: Number,
    mgmtPoolSize: Number,
    p2pPoolSize: Number
});

// Define cluster model.
exports.Cluster =  mongoose.model('Cluster', ClusterSchema);

//ClusterSchema.pre('save', function(next) {
//    // swap account model for the id
//    var id = this._doc.discovery._id;
//    //save the account model, which fires it's own middleware
//    this._doc.discovery.save();
//    // reset the account to the id before it is saved
//    this._doc.discovery = id;
//
//    next();
//});

ClusterSchema.pre('remove', function(next) {
    var discovery = false;

    if (this._doc && this._doc.discovery) discovery = {_id:this._doc.discovery._id};

    Discovery.remove(discovery, function(err) {
        if (err)
            next(err);

        next();
    });
});

// Define cache model.
exports.Cache =  mongoose.model('Cache', new Schema({
    space: { type: ObjectId, ref: 'Space' },
    name: String,
    mode: { type: String, enum: ['PARTITIONED', 'REPLICATED', 'LOCAL'] },
    backups: Number,
    atomicity: { type: String, enum: ['ATOMIC', 'TRANSACTIONAL'] },
    clusters: [{ type: ObjectId, ref: 'Cluster' }]
}));

exports.upsert = function(model, data, cb){
    if (data._id) {
        var id = data._id;

        delete data._id;

        model.findOneAndUpdate({_id: id}, data, cb);
    }
    else
        new model(data).save(cb);
};

exports.mongoose = mongoose;