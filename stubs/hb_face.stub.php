<?php

/** @generate-class-entries */

/**
 * An hb_blob_t. One PHP object per native pointer; dropping it does not call hb_blob_destroy().
 * fromPointer() trusts any address other than 0.
 */
final class hb_blob_t
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

/**
 * An hb_face_t. One PHP object per native pointer; dropping it does not call hb_face_destroy().
 * fromPointer() trusts any address other than 0.
 */
final class hb_face_t
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): static {}
}

/** A file that does not open answers the empty blob, as HarfBuzz does. */
function hb_blob_create_from_file(string $file_name): hb_blob_t {}

function hb_blob_destroy(hb_blob_t $blob): void {}

/** The number of faces in the blob: 0 for one that is not an OpenType font or collection. */
function hb_face_count(hb_blob_t $blob): int {}

/** The face references the blob: hb_blob_destroy() drops one reference. */
function hb_face_create(hb_blob_t $blob, int $index): hb_face_t {}

function hb_face_destroy(hb_face_t $face): void {}
