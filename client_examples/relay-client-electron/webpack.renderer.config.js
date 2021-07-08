const rules = require('./webpack.rules');

rules.push({
    test: /\.jsx?$/i,
    use: {
        loader: 'babel-loader',
        options: {
            exclude: /node_modules/,
            presets: ['@babel/preset-env', '@babel/preset-react'],
        },
    },
});
rules.push({
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
        },
    },
});
rules.push({
    test: /\.(ttf|png|svg|gif|jpg)$/i,
    use: { loader: 'file-loader' },
});
rules.push({
    test: /\.s?css$/,
    use: [
        { loader: 'style-loader' },
        { loader: 'css-loader' },
        { loader: 'sass-loader' },
    ],
});

module.exports = {
    // Put your normal webpack config below here
    module: {
        rules,
    },
    resolve: {
        extensions: ['.js', '.jsx', '.ts', '.tsx'],
    },
};
