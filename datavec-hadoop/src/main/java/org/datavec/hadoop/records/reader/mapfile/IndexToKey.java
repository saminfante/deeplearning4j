/*
 *  * Copyright 2016 Skymind, Inc.
 *  *
 *  *    Licensed under the Apache License, Version 2.0 (the "License");
 *  *    you may not use this file except in compliance with the License.
 *  *    You may obtain a copy of the License at
 *  *
 *  *        http://www.apache.org/licenses/LICENSE-2.0
 *  *
 *  *    Unless required by applicable law or agreed to in writing, software
 *  *    distributed under the License is distributed on an "AS IS" BASIS,
 *  *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  *    See the License for the specific language governing permissions and
 *  *    limitations under the License.
 */

package org.datavec.hadoop.records.reader.mapfile;

import org.apache.hadoop.io.MapFile;
import org.apache.hadoop.io.WritableComparable;

import java.io.IOException;

/**
 * Created by Alex on 29/05/2017.
 */
public interface IndexToKey {

    void initialize(MapFile.Reader reader) throws IOException;

    WritableComparable getKeyForIndex(long index);

    long getNumRecords(MapFile.Reader reader) throws IOException;

}
