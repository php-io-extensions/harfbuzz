<?php

declare(strict_types=1);

require_once __DIR__.'/../scripts/support.php';

it('counts the HB_EXTERN functions of each header, across lines, sorted, once each, not in comments or macros', function (): void {
    $header = "HB_EXTERN hb_buffer_t *\nhb_buffer_create (void);\n"
        ."HB_EXTERN void\nhb_shape (hb_font_t           *font,\n\t  hb_buffer_t         *buffer,\n\t  const hb_feature_t  *features,\n\t  unsigned int         num_features);\n"
        ."/* HB_EXTERN void hb_commented_out (void); */\n"
        ."#define HB_EXTERN extern\n"
        ."typedef void (*hb_destroy_func_t) (void *user_data);\n";

    expect(support_counted(['hb-shape.h' => $header]))
        ->toBe(['hb-shape.h' => ['hb_buffer_create', 'hb_shape']]);
});

it('measures libharfbuzz headers: the ones hb.h, hb-ot.h and hb-aat.h include, and hb-ft.h', function (): void {
    $hb = "#include \"hb-blob.h\"\n#include \"hb-buffer.h\"\n";
    $ot = "#include \"hb.h\"\n#include \"hb-ot-layout.h\"\n";

    expect(support_measured_headers(['hb.h' => $hb, 'hb-ot.h' => $ot, 'hb-aat.h' => "#include \"hb-aat-layout.h\"\n"]))
        ->toBe(['hb-aat-layout.h', 'hb-blob.h', 'hb-buffer.h', 'hb-ft.h', 'hb-ot-layout.h']);
});

it('reads the top-level functions of a stub, not class methods', function (): void {
    $stub = "<?php\nfunction hb_buffer_create(): hb_buffer_t {}\nfinal class hb_font_t { public function pointer(): int {} }\nfunction hb_shape(): void {}\n";

    expect(support_functions($stub))->toBe(['hb_buffer_create', 'hb_shape']);
});

it('renders one line per function and lists the bindings that match none', function (): void {
    $md = support_render(['hb-buffer.h' => ['hb_buffer_create', 'hb_buffer_destroy']], ['hb_buffer_create' => true, 'hb_helper' => true], 'HarfBuzz 8.3.0', '2026-11-05');

    expect($md)->toContain('- [x] `hb_buffer_create`')
        ->toContain('- [ ] `hb_buffer_destroy`')
        ->toContain('## hb-buffer.h (1 / 2)')
        ->toContain('HarfBuzz 8.3.0 — 1 / 2 members bound (50.0%) — 2026-11-05')
        ->toContain('- `hb_helper`');
});

it('computes the badge from the counts', function (): void {
    expect(support_badge('HarfBuzz API', 19, 700))
        ->toBe('[![HarfBuzz API](https://img.shields.io/badge/HarfBuzz_API-2.7%25_%2819%2F700%29-blue?style=flat-square)](SUPPORT.md)');
});

it('compares a support list without its date', function (): void {
    expect(support_same("HarfBuzz 8.3.0 — 2 / 4 members bound (50.0%) — 2026-11-05\n", "HarfBuzz 8.3.0 — 2 / 4 members bound (50.0%) — 2026-11-09\n"))->toBeTrue();
});
