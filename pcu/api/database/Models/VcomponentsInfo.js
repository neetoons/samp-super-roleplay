module.exports = (sequelize, DataTypes) => {
    return sequelize.define("vcomponents_info",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                primaryKey: true
            },
            part: {
                type: DataTypes.STRING(24),
                allowNull: true
            },
            name: {
                type: DataTypes.STRING(24),
                allowNull: true
            },
            pieces: {
                type: DataTypes.INTEGER,
                allowNull: true
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};