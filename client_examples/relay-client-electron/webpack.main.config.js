module.exports = {
    /**
     * This is the main entry point for your application, it's the first file
     * that runs in the main process.
     */
    entry: './src/main.js',
    // Put your normal webpack config below here
    module: {
        rules: [
            ...require('./webpack.rules'),
            ...[
                {
                    test: /\.ts/i,
                    use: {
                        loader: 'babel-loader',
                        options: {
                            presets: ['@babel/preset-typescript'],
                        },
                    },
                },
                {
                    test: /\.pem$|\.yaml/i,
                    use: { loader: 'file-loader' },
                },
            ],
        ],
    },
    resolve: {
        extensions: ['.js', '.jsx', '.ts', '.tsx'],
    },
};
