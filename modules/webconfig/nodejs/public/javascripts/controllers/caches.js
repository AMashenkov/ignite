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

configuratorModule.controller('cachesController', ['$scope', '$modal', '$http', function($scope, $modal, $http) {
        $scope.templates = [
            {value: {mode: 'PARTITIONED', atomicityMode: 'ATOMIC'}, label: 'Partitioned'},
            {value: {mode: 'REPLICATED', atomicityMode: 'ATOMIC'}, label: 'Replicated'},
            {value: {mode: 'LOCAL', atomicityMode: 'ATOMIC'}, label: 'Local'}
        ];

        $scope.atomicities = [
            {value: 'ATOMIC', label: 'Atomic'},
            {value: 'TRANSACTIONAL', label: 'Transactional'}
        ];

        $scope.modes = [
            {value: 'PARTITIONED', label: 'Partitioned'},
            {value: 'REPLICATED', label: 'Replicated'},
            {value: 'LOCAL', label: 'Local'}
        ];

        $scope.atomicWriteOrderModes = [
            {value: 'CLOCK', label: 'Clock'},
            {value: 'PRIMARY', label: 'Primary'}
        ];

        $scope.memoryModes = [
            {value: 'ONHEAP_TIERED', label: 'ONHEAP_TIERED'},
            {value: 'OFFHEAP_TIERED', label: 'OFFHEAP_TIERED'},
            {value: 'OFFHEAP_VALUES', label: 'OFFHEAP_VALUES'}
            ];

        $scope.rebalanceModes = [
            {value: 'SYNC', label: 'Synchronous'},
            {value: 'ASYNC', label: 'Asynchronous'},
            {value: 'NONE', label: 'None'}
        ];

        $scope.general = [];
        $scope.advanced = [];

        $http.get('/form-models/caches.json')
            .success(function(data) {
                $scope.general = data.general;
                $scope.advanced = data.advanced;
            });


        // Create popup for indexedTypes.
        var indexedTypesModal = $modal({scope: $scope, template: '/indexedTypes', show: false});

        $scope.editIndexedTypes = function(cache) {
            indexedTypesModal.$promise.then(indexedTypesModal.show);
        };

        $scope.caches = [];

        // When landing on the page, get caches and show them.
        $http.get('/rest/caches')
            .success(function(data) {
                $scope.spaces = data.spaces;
                $scope.caches = data.caches;
            });

        $scope.selectItem = function(item) {
            $scope.selectedItem = item;

            $scope.backupItem = angular.copy(item);
        };

        // Add new cache.
        $scope.createItem = function() {
            var item = angular.copy($scope.create.template);

            item.name = 'Cache ' + ($scope.caches.length + 1);
            item.space = $scope.spaces[0]._id;

            $http.post('/rest/caches/save', item)
                .success(function(_id) {
                    item._id = _id;

                    $scope.caches.push(item);

                    $scope.selectItem(item);
                })
                .error(function(errorMessage) {
                    console.log('Error: ' + errorMessage);
                });
        };

        $scope.removeItem = function(item) {
            $http.post('/rest/caches/remove', {_id: item._id})
                .success(function() {
                    var index = $scope.caches.indexOf(item);

                    if (index !== -1) {
                        $scope.caches.splice(index, 1);

                        if ($scope.selectedItem == item) {
                            $scope.selectedItem = undefined;

                            $scope.backupItem = undefined;
                        }
                    }
                })
                .error(function(errorMessage) {
                    console.log('Error: ' + errorMessage);
                });
        };

        // Save cache in db.
        $scope.saveItem = function(item) {
            $http.post('/rest/caches/save', item)
                .success(function() {
                    var i = _.findIndex($scope.caches, function(cache) {
                        return cache._id == item._id;
                    });

                    if (i >= 0)
                        angular.extend($scope.caches[i], item);

                    $scope.selectItem(item);
                })
                .error(function(errorMessage) {
                    console.log('Error: ' + errorMessage);
                });
        };
    }]
);