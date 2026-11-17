<?php

declare(strict_types=1);

// HarfBuzz's own font loading: hb_blob_create_from_file, hb_face_create, hb_font_create. The font reads
// the file with HarfBuzz's OpenType functions, at the face's units per em until hb_font_set_scale.
// Menlo and DejaVu Sans Mono both have 2048 units per em and advance 1233.

/** Shapes $text with $font: the glyph ids and x advances. */
function shapeWith(hb_font_t $font, string $text): array
{
    $buffer = hb_buffer_create();
    hb_buffer_add_utf8($buffer, $text, -1, 0, -1);
    hb_buffer_guess_segment_properties($buffer);
    hb_shape($font, $buffer, null, 0);

    return [
        array_map(fn (hb_glyph_info_t $i): int => $i->codepoint, hb_buffer_get_glyph_infos($buffer, $count)),
        array_map(fn (hb_glyph_position_t $p): int => $p->x_advance, hb_buffer_get_glyph_positions($buffer, $count)),
    ];
}

it('shapes a file opened without FreeType, at the face units per em', function (): void {
    $blob = hb_blob_create_from_file(monoFont());
    $face = hb_face_create($blob, 0);
    $font = hb_font_create($face);

    [$glyphs, $advances] = shapeWith($font, 'aba');

    expect($blob)->toBeInstanceOf(hb_blob_t::class)
        ->and($face)->toBeInstanceOf(hb_face_t::class)
        ->and($advances)->toBe([1233, 1233, 1233])
        ->and($glyphs[0])->toBe($glyphs[2])
        ->and($glyphs[0])->not->toBe($glyphs[1])
        ->and($glyphs[0])->toBeGreaterThan(0);
});

it('scales the advances of a font hb_font_create made', function (): void {
    $font = hb_font_create(hb_face_create(hb_blob_create_from_file(monoFont()), 0));
    hb_font_set_scale($font, 4096, 4096);

    expect(shapeWith($font, 'a')[1])->toBe([2466]);
});

it('passes the face index: in a collection, one past its faces has no glyphs, so text shapes as glyph 0', function (): void {
    $collection = hb_blob_create_from_file(collectionOfOne());
    $first = hb_font_create(hb_face_create($collection, 0));
    $past = hb_font_create(hb_face_create($collection, 1));

    expect(hb_face_count($collection))->toBe(1)
        ->and(shapeWith($first, 'a')[0][0])->toBeGreaterThan(0)
        ->and(shapeWith($past, 'a')[0])->toBe([0]);
});

it('passes the face index of a single font as given: HarfBuzz reads any index of it as its one face', function (): void {
    $blob = hb_blob_create_from_file(singleFont());
    $glyph = shapeWith(hb_font_create(hb_face_create($blob, 0)), 'a')[0];

    expect($glyph[0])->toBeGreaterThan(0)
        ->and(shapeWith(hb_font_create(hb_face_create($blob, 7)), 'a')[0])->toBe($glyph);
});

it('answers the empty blob for a file that does not open, whose face has no glyphs', function (): void {
    $blob = hb_blob_create_from_file(sys_get_temp_dir().'/no-such-font-'.getmypid().'.ttf');
    $font = hb_font_create(hb_face_create($blob, 0));

    expect($blob)->toBeInstanceOf(hb_blob_t::class)
        ->and(shapeWith($font, 'a')[0])->toBe([0]);
});

it('releases blob and face handles on destroy, and passes a second destroy through as NULL', function (): void {
    $blob = hb_blob_create_from_file(monoFont());
    $face = hb_face_create($blob, 0);

    expect(hb_blob_t::fromPointer($blob->pointer()))->toBe($blob)
        ->and(hb_face_t::fromPointer($face->pointer()))->toBe($face);

    hb_face_destroy($face);
    hb_blob_destroy($blob);
    hb_face_destroy($face);
    hb_blob_destroy($blob);

    expect($blob->pointer())->toBe(0)
        ->and($face->pointer())->toBe(0);
});

it('passes a destroyed blob and face to HarfBuzz as NULL, as fromPointer(0) handles are passed', function (): void {
    $blob = hb_blob_create_from_file(monoFont());
    hb_blob_destroy($blob);
    $face = hb_face_create($blob, 0);
    $nullBlobFace = hb_face_create(hb_blob_t::fromPointer(0), 0);
    hb_face_destroy($face);

    expect(shapeWith(hb_font_create(hb_face_create($blob, 0)), 'a'))->toBe(shapeWith(hb_font_create($nullBlobFace), 'a'))
        ->and(shapeWith(hb_font_create($face), 'a'))->toBe(shapeWith(hb_font_create(hb_face_t::fromPointer(0)), 'a'))
        ->and(shapeWith(hb_font_create($face), 'a'))->not->toBe(shapeWith(hb_font_create(hb_face_create(hb_blob_create_from_file(monoFont()), 0)), 'a'));
});

it('shapes after the blob and face are destroyed and dropped: the font holds its own references', function (): void {
    $run = hbChild(sprintf(<<<'PHP'
        $open = static function (): hb_font_t {
            $blob = hb_blob_create_from_file('%s');
            $face = hb_face_create($blob, 0);
            hb_blob_destroy($blob);
            $own = hb_font_create($face);
            hb_face_destroy($face);

            return $own;
        };
        $own = $open();
        gc_collect_cycles();
        $text = hb_buffer_create();
        hb_buffer_add_utf8($text, 'abc', -1, 0, -1);
        hb_buffer_guess_segment_properties($text);
        hb_shape($own, $text, null, 0);

        return array_map(fn (hb_glyph_position_t $p): int => $p->x_advance, hb_buffer_get_glyph_positions($text, $count));
    PHP, monoFont()));

    expect($run['signal'])->toBeNull()
        ->and($run['result'])->toBe([1233, 1233, 1233]);
});

it('counts the faces of a blob: those of a font file, 0 for a file that is not a font or does not open', function (): void {
    $missing = hb_blob_create_from_file(sys_get_temp_dir().'/no-such-font-'.getmypid().'.ttf');
    $text = tempnam(sys_get_temp_dir(), 'hb-text');
    file_put_contents($text, 'not a font at all');
    $notFont = hb_blob_create_from_file($text);
    unlink($text);

    expect(hb_face_count(hb_blob_create_from_file(monoFont())))->toBe(PHP_OS_FAMILY === 'Darwin' ? 4 : 1)
        ->and(hb_face_count($notFont))->toBe(0)
        ->and(hb_face_count($missing))->toBe(0)
        ->and(hb_face_count(hb_blob_t::fromPointer(0)))->toBe(0);
});
