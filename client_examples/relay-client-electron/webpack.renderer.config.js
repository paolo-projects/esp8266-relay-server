rules = [
    ...require('./webpack.rules'),
    ...[
        {
            test: /\.jsx?$/i,
            use: {
                loader: 'babel-loader',
                options: {
                    exclude: /node_modules/,
                    presets: ['@babel/preset-env', '@babel/preset-react'],
                },
            },
        },
        {
            test: /\.tsx?$/i,
            use: {
                loader: 'babel-loader',
                options: {
                    exclude: /node_modules/,
                    presets: [
                        '@babel/preset-env',
                        '@babel/preset-react',
                        '@babel/preset-typescript',
                    ],
                    plugins: ['@emotion'],
                },
            },
        },
        {
            test: /\.(ttf|png|svg|gif|jpg)$/i,
            use: { loader: 'file-loader' },
        },
        {
            test: /\.s?css$/,
            use: [
                { loader: 'style-loader' },
                { loader: 'css-loader' },
                { loader: 'sass-loader' },
            ],
        },
    ],
];

module.exports = {
    // Put your normal webpack config below here
    module: {
        rules,
    },
    resolve: {
        extensions: ['.js', '.jsx', '.ts', '.tsx'],
    },
};
