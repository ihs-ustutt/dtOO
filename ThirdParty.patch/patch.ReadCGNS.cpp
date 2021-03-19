diff --git a/src/io/ReadCGNS.cpp b/src/io/ReadCGNS.cpp
index 968beca55..9531562f2 100644
--- a/src/io/ReadCGNS.cpp
+++ b/src/io/ReadCGNS.cpp
@@ -467,8 +467,17 @@ ErrorCode ReadCGNS::create_elements( char* sectionName, const Tag* file_id_tag,
 
     result = readMeshIface->get_element_connect( elems_count, verts_per_elem, ent_type, 1, handle, conn_array );MB_CHK_SET_ERR( result, fileName << ": Trouble reading elements" );
 
-    memcpy( conn_array, &elemsConn[0], elemsConn.size() * sizeof( EntityHandle ) );
 
+    if (sizeof(EntityHandle)==sizeof(cgsize_t)) {
+      memcpy(conn_array, &elemsConn[0], elemsConn.size() * sizeof(EntityHandle));
+    }
+    else {
+      std::vector< EntityHandle > elemsConnTwin(elemsConn.size(), 0);
+      for ( int i=0; i<elemsConn.size(); i++ ) {
+        elemsConnTwin[ i ] = static_cast< EntityHandle >(elemsConn[ i ]);
+      }
+      memcpy(conn_array, &elemsConnTwin[0], elemsConnTwin.size() * sizeof(EntityHandle));
+    } 
     // Notify MOAB of the new elements
     result = readMeshIface->update_adjacencies( handle, elems_count, verts_per_elem, conn_array );
     if( MB_SUCCESS != result ) return result;
