/*
* Copyright 2018 Coati Software KG
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef SOURCETRAIL_DATABASE_STORAGE_H
#define SOURCETRAIL_DATABASE_STORAGE_H

#include <memory>
#include <string>
#include <vector>

#include "CppSQLite3.h"

#include "StorageEdge.h"
#include "StorageError.h"
#include "StorageFile.h"
#include "StorageLocalSymbol.h"
#include "StorageNode.h"
#include "StorageOccurrence.h"
#include "StorageSourceLocation.h"
#include "StorageSymbol.h"

namespace sourcetrail
{
	/**
	 * Class wrapping the write and update interface of the Sourcetrail database.
	 *
	 * The DatabaseStorage provides the interface for writing and updating the Sourcetrail database. This interface only
	 * knows about basic data types, more elaborate types like enums, structs and classes need to be converted to basic
	 * types before using this interface.
	*/
	class DatabaseStorage
	{
	public:
		static int getSupportedDatabaseVersion();
		static std::shared_ptr<DatabaseStorage> openDatabase(const std::string& dbFilePath);
		~DatabaseStorage();

		void setupTables();
		void clearTables();

		bool isEmpty() const;
		bool isCompatible() const;
		int getLoadedDatabaseVersion() const;
		void beginTransaction();
		void commitTransaction();
		void rollbackTransaction();
		void optimizeDatabaseMemory();

		int addNode(const StorageNodeData& storageNodeData);
		void addSymbol(const StorageSymbol& storageSymbol);
		void addFile(const StorageFile& storageFile);
		int addEdge(const StorageEdgeData& storageEdgeData);
		int addLocalSymbol(const StorageLocalSymbolData& storageLocalSymbolData);
		int addSourceLocation(const StorageSourceLocationData& storageSourceLocationData);
		void addOccurrence(const StorageOccurrence& storageOccurrence);
		int addError(const StorageErrorData& storageErrorData);

		void setNodeType(int nodeId, int nodeKind);

		template <typename ResultType>
		std::vector<ResultType> getAll() const
		{
			return doGetAll<ResultType>("");
		}

	private:
		DatabaseStorage() = default;

		void insertOrUpdateMetaValue(const std::string& key, const std::string& value);
		void executeStatement(const std::string& statement) const;
		void executeStatement(CppSQLite3Statement& statement) const;
		CppSQLite3Query executeQuery(const std::string& query) const;
		CppSQLite3Query executeQuery(CppSQLite3Statement& statement) const;

		template <typename ResultType>
		std::vector<ResultType> doGetAll(const std::string& query) const;

		mutable CppSQLite3DB m_database;
	};

	template <>
	std::vector<StorageEdge> DatabaseStorage::doGetAll<StorageEdge>(const std::string& query) const;
	template <>
	std::vector<StorageNode> DatabaseStorage::doGetAll<StorageNode>(const std::string& query) const;
	template <>
	std::vector<StorageSymbol> DatabaseStorage::doGetAll<StorageSymbol>(const std::string& query) const;
	template <>
	std::vector<StorageFile> DatabaseStorage::doGetAll<StorageFile>(const std::string& query) const;
	template <>
	std::vector<StorageLocalSymbol> DatabaseStorage::doGetAll<StorageLocalSymbol>(const std::string& query) const;
	template <>
	std::vector<StorageSourceLocation> DatabaseStorage::doGetAll<StorageSourceLocation>(const std::string& query) const;
	template <>
	std::vector<StorageOccurrence> DatabaseStorage::doGetAll<StorageOccurrence>(const std::string& query) const;
	template <>
	std::vector<StorageError> DatabaseStorage::doGetAll<StorageError>(const std::string& query) const;
}

#endif // SOURCETRAIL_DATABASE_STORAGE_H
