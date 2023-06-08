// --------------------------------------------------------------------------------------------------------
// Configuration
// --------------------------------------------------------------------------------------------------------

computing_generate_main_space__deps = ['$GL'];
computing_animate_latent_space__deps = ['$GL'];

let all_model_info = {
    dcgan64: {
        description: 'DCGAN, 64x64 (16 MB)',
        model_url: "https://storage.googleapis.com/store.alantian.net/tfjs_gan/chainer-dcgan-celebahq-64/tfjs_SmoothedGenerator_50000/model.json",
        model_size: 64,
        model_latent_dim: 128,
        draw_multiplier: 4,
        animate_frame: 200,
    },
    resnet128: {
        description: 'ResNet, 128x128 (252 MB)',
        model_url: "https://storage.googleapis.com/store.alantian.net/tfjs_gan/chainer-resent128-celebahq-128/tfjs_SmoothedGenerator_20000/model.json",
        model_size: 128,
        model_latent_dim: 128,
        draw_multiplier: 2,
        animate_frame: 10
    },
    resnet256: {
        description: 'ResNet, 256x256 (252 MB)',
        model_url: "https://storage.googleapis.com/store.alantian.net/tfjs_gan/chainer-resent256-celebahq-256/tfjs_SmoothedGenerator_40000/model.json",
        model_size: 256,
        model_latent_dim: 128,
        draw_multiplier: 1,
        animate_frame: 10
    }
};

// --------------------------------------------------------------------------------------------------------
// Computing
// --------------------------------------------------------------------------------------------------------

function computing_prep_canvas(size) {
    // We don't `return tf.image.resizeBilinear(v1, [size * draw_multiplier, size * draw_multiplier]);`
    // since that makes image blurred, which is not what we want.
    // So instead, we manually enlarge the image.
    canvas2 = document.createElement("canvas");
    let ctx = canvas2.getContext("2d");
    ctx.canvas.width = size;
    ctx.canvas.height = size;
}

function image_enlarge(y, draw_multiplier) {
    if (draw_multiplier === 1) {
        return y;
    }
    let size = y.shape[0];
    return y.expandDims(2).tile([1, 1, draw_multiplier, 1]
    ).reshape([size, size * draw_multiplier, 3]
    ).expandDims(1).tile([1, draw_multiplier, 1, 1]
    ).reshape([size * draw_multiplier, size * draw_multiplier, 3])
}

async function computing_animate_latent_space(model, draw_multiplier, animate_frame, textureID) {
    const inputShape = model.inputs[0].shape.slice(1);
    const shift = tf.randomNormal(inputShape).expandDims(0);
    const freq = tf.randomNormal(inputShape, 0, .1).expandDims(0);

    let c = canvas2;
    let i = 0;
    // while (i < animate_frame) {
    while (animate_frame) {
        i++;
        const y = tf.tidy(() => {
            const z = tf.sin(tf.scalar(i).mul(freq).add(shift));
            const y = model.predict(z).squeeze().transpose([1, 2, 0]).div(tf.scalar(2)).add(tf.scalar(.5));
            return image_enlarge(y, draw_multiplier);
        });

        await tf.browser.toPixels(y, c);
        tf.dispose(y);
	GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureID]);
	GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, c);
	GLctx.bindTexture(GLctx.TEXTURE_2D, null);
        await tf.nextFrame();
    }
}

async function computing_generate_main(model, size, draw_multiplier, latent_dim, textureID) {
    const y = tf.tidy(() => {
        const z = tf.randomNormal([1, latent_dim]);
        const y = model.predict(z).squeeze().transpose([1, 2, 0]).div(tf.scalar(2)).add(tf.scalar(0.5));
        return image_enlarge(y, draw_multiplier);
    });
    let c = canvas2;
    await tf.browser.toPixels(y, c);
        tf.dispose(y);
	GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[textureID]);
	GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, c);
	GLctx.bindTexture(GLctx.TEXTURE_2D, null);
}

const ui_delay_before_tf_computing_ms = 2000;  // Delay that many ms before tf computing, which can block UI drawing.

function resolve_after_ms(x, ms) {
    return new Promise(resolve => {
        setTimeout(() => {
            resolve(x);
        }, ms);
    });
}

class ModelRunner {
    constructor() {
        this.model_promise_cache = {};
        this.model_promise = null;
        this.model_name = null;
        this.start_time = null;
    }

    setup_model(model_name) {
        this.model_name = model_name;
        let model_info = all_model_info[model_name];
        let model_size = model_info.model_size,
            model_url = model_info.model_url,
            draw_multiplier = model_info.draw_multiplier,
            description = model_info.description;

        computing_prep_canvas(model_size * draw_multiplier);

        if (model_name in this.model_promise_cache) {
            this.model_promise = this.model_promise_cache[model_name];
        } else {
            this.model_promise = tf.loadLayersModel(model_url);
            this.model_promise.then((model) => {
                return resolve_after_ms(model, ui_delay_before_tf_computing_ms);
            });
            this.model_promise_cache[model_name] = this.model_promise;
        }
    }

    generate(textureID) {
        let model_info = all_model_info[this.model_name];
        let model_size = model_info.model_size,
            model_latent_dim = model_info.model_latent_dim,
            draw_multiplier = model_info.draw_multiplier;

        computing_prep_canvas(model_size * draw_multiplier);
        console.log('Generating image...');

        this.model_promise.then((model) => {
            return resolve_after_ms(model, ui_delay_before_tf_computing_ms);
        }).then((model) => {
            this.start_ms = (new Date()).getTime();
            return computing_generate_main(model, model_size, draw_multiplier, model_latent_dim, textureID);
        }).then((_) => {
            let end_ms = (new Date()).getTime();
            console.log(`Image generated. It took ${(end_ms - this.start_ms)} ms.`);
            Module.isGenerated(`Animation generated. It took ${(end_ms - this.start_ms)} ms.`);
        });
    }

    animate(textureID) {
        let model_info = all_model_info[this.model_name];
        let model_size = model_info.model_size,
            model_latent_dim = model_info.model_latent_dim,
            draw_multiplier = model_info.draw_multiplier,
            animate_frame = model_info.animate_frame;

        computing_prep_canvas(model_size * draw_multiplier);
        console.log('Generating animation...');
        this.start_ms = (new Date()).getTime();
        this.model_promise.then((model) => {
            return resolve_after_ms(model, ui_delay_before_tf_computing_ms);
        }).then((model) => {
            let end_ms = (new Date()).getTime();
            console.log(`Animation generated. It took ${(end_ms - this.start_ms)} ms.`);
            Module.isGenerated(`Animation generated. It took ${(end_ms - this.start_ms)} ms.`);
            return computing_animate_latent_space(model, draw_multiplier, animate_frame, textureID);
        });
    }
}

let model_runner = new ModelRunner();
